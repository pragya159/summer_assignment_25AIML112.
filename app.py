import os
import uuid
import secrets
from functools import wraps
from flask import Flask, request, jsonify, render_template, session, send_from_directory
from flask_cors import CORS
from werkzeug.security import generate_password_hash, check_password_hash

from database import get_db, init_db
from services.mining_service import calculate_roi, create_mining_contract, accrue_profits, PLATFORM_FEE_RATE
from services.payment_service import create_deposit_order, verify_payment, process_withdrawal
from services.chatbot_service import get_chatbot_response

# App initialization
app = Flask(__name__, template_folder="templates", static_folder="static")
app.secret_key = os.environ.get("SECRET_KEY", secrets.token_hex(32))
CORS(app, supports_credentials=True)

# In-memory auth tokens for REST API clients
ACTIVE_TOKENS = {}

def require_auth(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        user_id = session.get("user_id")
        
        # Check authorization header if not in session
        auth_header = request.headers.get("Authorization")
        if not user_id and auth_header and auth_header.startswith("Bearer "):
            token = auth_header.split(" ")[1]
            user_id = ACTIVE_TOKENS.get(token)

        if not user_id:
            return jsonify({"success": False, "error": "Authentication required. Please login."}), 401

        request.current_user_id = user_id
        return f(*args, **kwargs)
    return decorated

# ----------------- PUBLIC & SYSTEM ROUTES ----------------- #

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/api/platform/stats", methods=["GET"])
def platform_stats():
    """
    Returns public platform metrics (Global network hashrate, total paid out, etc.)
    """
    conn = get_db()
    cursor = conn.cursor()
    cursor.execute("SELECT COUNT(*) FROM users")
    total_miners = cursor.fetchone()[0] + 12840  # Add base simulated platform participants

    cursor.execute("SELECT COALESCE(SUM(total_withdrawn), 0) FROM wallets")
    total_paid_out = cursor.fetchone()[0] + 4825900.0  # Simulated historical baseline in USD

    cursor.execute("SELECT COALESCE(SUM(fee_amount), 0) FROM platform_revenue")
    total_platform_revenue = cursor.fetchone()[0]

    cursor.execute("SELECT COALESCE(SUM(active_hashrate), 0) FROM wallets")
    active_hashrate = cursor.fetchone()[0] + 1420500.0  # TH/s baseline (1.42 EH/s)
    conn.close()

    return jsonify({
        "success": True,
        "stats": {
            "global_hashrate_ehs": round(active_hashrate / 1000000.0, 2),
            "total_miners": total_miners,
            "total_paid_out_usd": round(total_paid_out, 2),
            "uptime_pct": 99.98,
            "platform_fee_pct": PLATFORM_FEE_RATE * 100,
            "platform_revenue_collected": round(total_platform_revenue, 4),
            "renewable_energy_pct": 100
        }
    })

# ----------------- AUTHENTICATION ROUTES ----------------- #

@app.route("/api/auth/register", methods=["POST"])
def register():
    data = request.get_json() or {}
    username = data.get("username", "").strip()
    email = data.get("email", "").strip().lower()
    password = data.get("password", "")

    if not username or len(username) < 3:
        return jsonify({"success": False, "error": "Username must be at least 3 characters long."}), 400
    if not email or "@" not in email:
        return jsonify({"success": False, "error": "Valid email address is required."}), 400
    if not password or len(password) < 6:
        return jsonify({"success": False, "error": "Password must be at least 6 characters long."}), 400

    conn = get_db()
    cursor = conn.cursor()

    cursor.execute("SELECT id FROM users WHERE username = ? OR email = ?", (username, email))
    if cursor.fetchone():
        conn.close()
        return jsonify({"success": False, "error": "Username or email is already registered."}), 409

    # Generate User ID (e.g. EM-74921)
    user_id = f"EM-{secrets.randbelow(90000) + 10000}"
    password_hash = generate_password_hash(password)

    cursor.execute("""
    INSERT INTO users (user_id, username, email, password_hash)
    VALUES (?, ?, ?, ?)
    """, (user_id, username, email, password_hash))

    # Initialize user wallet
    cursor.execute("""
    INSERT INTO wallets (user_id, usdt_balance, usd_balance, total_deposited, total_withdrawn, total_profit, active_hashrate)
    VALUES (?, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
    """, (user_id,))

    conn.commit()
    conn.close()

    # Session & token
    session["user_id"] = user_id
    token = secrets.token_hex(24)
    ACTIVE_TOKENS[token] = user_id

    return jsonify({
        "success": True,
        "message": "Account created successfully!",
        "token": token,
        "user": {
            "user_id": user_id,
            "username": username,
            "email": email
        }
    }), 201

@app.route("/api/auth/login", methods=["POST"])
def login():
    data = request.get_json() or {}
    login_id = data.get("login", "").strip()
    password = data.get("password", "")

    if not login_id or not password:
        return jsonify({"success": False, "error": "Username/email and password are required."}), 400

    conn = get_db()
    cursor = conn.cursor()
    cursor.execute("""
    SELECT user_id, username, email, password_hash, role
    FROM users 
    WHERE username = ? OR email = ?
    """, (login_id, login_id.lower()))
    user = cursor.fetchone()
    conn.close()

    if not user or not check_password_hash(user["password_hash"], password):
        return jsonify({"success": False, "error": "Invalid username/email or password."}), 401

    session["user_id"] = user["user_id"]
    token = secrets.token_hex(24)
    ACTIVE_TOKENS[token] = user["user_id"]

    return jsonify({
        "success": True,
        "message": "Login successful!",
        "token": token,
        "user": {
            "user_id": user["user_id"],
            "username": user["username"],
            "email": user["email"],
            "role": user["role"]
        }
    })

@app.route("/api/auth/me", methods=["GET"])
@require_auth
def get_current_user():
    user_id = request.current_user_id
    conn = get_db()
    cursor = conn.cursor()
    cursor.execute("SELECT user_id, username, email, role, created_at FROM users WHERE user_id = ?", (user_id,))
    user = cursor.fetchone()
    conn.close()

    if not user:
        return jsonify({"success": False, "error": "User not found"}), 404

    return jsonify({
        "success": True,
        "user": dict(user)
    })

@app.route("/api/auth/logout", methods=["POST"])
def logout():
    session.pop("user_id", None)
    auth_header = request.headers.get("Authorization")
    if auth_header and auth_header.startswith("Bearer "):
        token = auth_header.split(" ")[1]
        ACTIVE_TOKENS.pop(token, None)
    return jsonify({"success": True, "message": "Logged out successfully."})

# ----------------- MINING PLANS & CALCULATOR ----------------- #

@app.route("/api/plans", methods=["GET"])
def list_plans():
    conn = get_db()
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM mining_plans ORDER BY min_investment ASC")
    rows = cursor.fetchall()
    conn.close()
    return jsonify({
        "success": True,
        "plans": [dict(r) for r in rows]
    })

@app.route("/api/calculator/calculate", methods=["POST"])
def calculate():
    data = request.get_json() or {}
    amount = float(data.get("amount", 100))
    plan_code = data.get("plan_code", "starter")
    duration = int(data.get("duration", 30))

    try:
        roi = calculate_roi(amount, plan_code, duration)
        return jsonify({"success": True, "data": roi})
    except Exception as e:
        return jsonify({"success": False, "error": str(e)}), 400

# ----------------- WALLET & DASHBOARD ----------------- #

@app.route("/api/wallet/summary", methods=["GET"])
@require_auth
def wallet_summary():
    user_id = request.current_user_id
    
    # Auto-accrue any uncollected yields
    accrue_profits(user_id)

    conn = get_db()
    cursor = conn.cursor()

    cursor.execute("SELECT * FROM wallets WHERE user_id = ?", (user_id,))
    wallet = cursor.fetchone()

    cursor.execute("SELECT COUNT(*) FROM contracts WHERE user_id = ? AND status = 'ACTIVE'", (user_id,))
    active_contracts_count = cursor.fetchone()[0]

    cursor.execute("SELECT COALESCE(SUM(fee_amount), 0) FROM platform_revenue WHERE user_id = ?", (user_id,))
    total_fee_paid = cursor.fetchone()[0]

    conn.close()

    if not wallet:
        return jsonify({"success": False, "error": "Wallet not found."}), 404

    return jsonify({
        "success": True,
        "wallet": {
            "usdt_balance": round(wallet["usdt_balance"], 4),
            "usd_balance": round(wallet["usd_balance"], 2),
            "total_balance_usd": round(wallet["usd_balance"] + wallet["usdt_balance"], 2),
            "active_hashrate_ths": round(wallet["active_hashrate"], 2),
            "total_deposited": round(wallet["total_deposited"], 2),
            "total_withdrawn": round(wallet["total_withdrawn"], 2),
            "total_profit": round(wallet["total_profit"], 4),
            "platform_fee_paid_2pct": round(total_fee_paid, 4),
            "active_contracts_count": active_contracts_count
        }
    })

@app.route("/api/contracts/active", methods=["GET"])
@require_auth
def list_active_contracts():
    user_id = request.current_user_id
    accrue_profits(user_id)

    conn = get_db()
    cursor = conn.cursor()
    cursor.execute("""
    SELECT * FROM contracts 
    WHERE user_id = ? 
    ORDER BY id DESC
    """, (user_id,))
    contracts = cursor.fetchall()
    conn.close()

    result = []
    for c in contracts:
        cdict = dict(c)
        # Calculate percent completed
        start = cdict["start_date"]
        end = cdict["end_date"]
        result.append(cdict)

    return jsonify({
        "success": True,
        "contracts": result
    })

@app.route("/api/contracts/create", methods=["POST"])
@require_auth
def deploy_contract_from_wallet():
    """
    Deploys a new mining contract directly from existing wallet balance.
    """
    user_id = request.current_user_id
    data = request.get_json() or {}
    plan_code = data.get("plan_code")
    amount = float(data.get("amount", 0))
    currency = data.get("currency", "USDT")

    if amount <= 0:
        return jsonify({"success": False, "error": "Invalid investment amount."}), 400

    conn = get_db()
    cursor = conn.cursor()
    cursor.execute("SELECT usdt_balance, usd_balance FROM wallets WHERE user_id = ?", (user_id,))
    wallet = cursor.fetchone()

    if not wallet:
        conn.close()
        return jsonify({"success": False, "error": "Wallet not found."}), 404

    current_balance = wallet["usdt_balance"] if currency == "USDT" else wallet["usd_balance"]
    if current_balance < amount:
        conn.close()
        return jsonify({
            "success": False, 
            "error": f"Insufficient {currency} balance ({current_balance:.2f} {currency}). Please deposit funds first."
        }), 400

    # Deduct balance
    if currency == "USDT":
        cursor.execute("UPDATE wallets SET usdt_balance = usdt_balance - ? WHERE user_id = ?", (amount, user_id))
    else:
        cursor.execute("UPDATE wallets SET usd_balance = usd_balance - ? WHERE user_id = ?", (amount, user_id))
    conn.commit()
    conn.close()

    try:
        contract = create_mining_contract(user_id, plan_code, amount, currency)
        return jsonify({
            "success": True,
            "message": f"Contract {contract['contract_id']} activated! Rigs are now hashing.",
            "contract": contract
        })
    except Exception as e:
        # Refund if failure
        conn = get_db()
        cursor = conn.cursor()
        if currency == "USDT":
            cursor.execute("UPDATE wallets SET usdt_balance = usdt_balance + ? WHERE user_id = ?", (amount, user_id))
        else:
            cursor.execute("UPDATE wallets SET usd_balance = usd_balance + ? WHERE user_id = ?", (amount, user_id))
        conn.commit()
        conn.close()
        return jsonify({"success": False, "error": str(e)}), 400

# ----------------- PAYMENTS & GATEWAY INTEGRATION ----------------- #

@app.route("/api/payments/checkout", methods=["POST"])
@require_auth
def create_checkout_order():
    """
    Initiates 'Pay on website' deposit order with unique TXID,
    addresses, and initial 'PENDING' status.
    """
    user_id = request.current_user_id
    data = request.get_json() or {}
    amount = float(data.get("amount", 0))
    currency = data.get("currency", "USDT")
    payment_method = data.get("payment_method", "USDT_TRC20")
    plan_code = data.get("plan_code")

    if amount <= 0:
        return jsonify({"success": False, "error": "Amount must be greater than zero."}), 400

    try:
        order = create_deposit_order(
            user_id=user_id,
            amount=amount,
            currency=currency,
            payment_method=payment_method,
            plan_code=plan_code
        )
        return jsonify({
            "success": True,
            "order": order
        })
    except Exception as e:
        return jsonify({"success": False, "error": str(e)}), 400

@app.route("/api/payments/verify", methods=["POST"])
@require_auth
def verify_deposit():
    """
    Payment gateway auto-verification API.
    Simulates blockchain confirmation / bank gateway authorization,
    transitions status to 'CREDITED' or 'FAILED', and updates balances.
    """
    data = request.get_json() or {}
    tx_hash = data.get("tx_hash")
    simulate_fail = bool(data.get("simulate_fail", False))

    if not tx_hash:
        return jsonify({"success": False, "error": "Transaction hash is required."}), 400

    try:
        result = verify_payment(tx_hash, simulate_failure=simulate_fail)
        return jsonify({
            "success": True,
            "result": result
        })
    except Exception as e:
        return jsonify({"success": False, "error": str(e)}), 400

@app.route("/api/payments/history", methods=["GET"])
@require_auth
def payment_history():
    user_id = request.current_user_id
    conn = get_db()
    cursor = conn.cursor()
    cursor.execute("""
    SELECT * FROM transactions 
    WHERE user_id = ? 
    ORDER BY id DESC 
    LIMIT 50
    """, (user_id,))
    txs = cursor.fetchall()
    conn.close()

    return jsonify({
        "success": True,
        "transactions": [dict(tx) for tx in txs]
    })

# ----------------- PROFIT & PAYOUT SYSTEM ----------------- #

@app.route("/api/profits/accrue", methods=["POST"])
@require_auth
def trigger_profit_accrual():
    """
    Manual/periodic endpoint that calculates accrued profit,
    applies the transparent 2% platform fee, and deposits net yield into wallet.
    """
    user_id = request.current_user_id
    result = accrue_profits(user_id)
    return jsonify({
        "success": True,
        "accrual": result
    })

@app.route("/api/payouts/request", methods=["POST"])
@require_auth
def request_payout():
    """
    Handles user payout / withdrawal request.
    """
    user_id = request.current_user_id
    data = request.get_json() or {}
    amount = float(data.get("amount", 0))
    currency = data.get("currency", "USDT")
    address = data.get("address", "").strip()

    try:
        payout = process_withdrawal(user_id, amount, currency, address)
        return jsonify({
            "success": True,
            "payout": payout
        })
    except Exception as e:
        return jsonify({"success": False, "error": str(e)}), 400

# ----------------- CUSTOMER SUPPORT & CHATBOT ----------------- #

@app.route("/api/support/tickets", methods=["GET", "POST"])
@require_auth
def handle_tickets():
    user_id = request.current_user_id
    conn = get_db()
    cursor = conn.cursor()

    if request.method == "POST":
        data = request.get_json() or {}
        subject = data.get("subject", "").strip()
        category = data.get("category", "General").strip()
        message = data.get("message", "").strip()

        if not subject or not message:
            conn.close()
            return jsonify({"success": False, "error": "Subject and message are required."}), 400

        ticket_id = f"TICK-{uuid.uuid4().hex[:8].upper()}"
        cursor.execute("""
        INSERT INTO support_tickets (ticket_id, user_id, subject, category, message, status)
        VALUES (?, ?, ?, ?, ?, 'OPEN')
        """, (ticket_id, user_id, subject, category, message))
        conn.commit()
        conn.close()

        return jsonify({
            "success": True,
            "message": "Support ticket created. Our engineering desk will respond shortly.",
            "ticket_id": ticket_id
        }), 201

    cursor.execute("""
    SELECT * FROM support_tickets WHERE user_id = ? ORDER BY id DESC
    """, (user_id,))
    tickets = cursor.fetchall()
    conn.close()
    return jsonify({
        "success": True,
        "tickets": [dict(t) for t in tickets]
    })

@app.route("/api/chatbot/message", methods=["POST"])
def chatbot_message():
    data = request.get_json() or {}
    message = data.get("message", "").strip()
    user_id = session.get("user_id")

    if not message:
        return jsonify({"success": False, "error": "Message cannot be empty."}), 400

    response = get_chatbot_response(message, user_id)
    return jsonify({
        "success": True,
        "reply": response["reply"],
        "suggestions": response.get("suggestions", [])
    })

if __name__ == "__main__":
    init_db()
    port = int(os.environ.get("PORT", 5000))
    print(f"Starting E-Mining Platform server at http://127.0.0.1:{port}")
    app.run(host="0.0.0.0", port=port, debug=False)
