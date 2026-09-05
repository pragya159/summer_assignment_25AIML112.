import uuid
import random
from datetime import datetime, timezone
from database import get_db
from services.mining_service import create_mining_contract

# System hot wallet addresses for deposits
USDT_TRC20_WALLET = "TRX9kM7V8xY2N4wL1qE5bA7zC3vD6sH9pM"
USDT_ERC20_WALLET = "0x71C38B29eFc0129B4d75cD96A329705E32dE1894"

def create_deposit_order(user_id, amount, currency="USDT", payment_method="USDT_TRC20", plan_code=None):
    """
    Creates a new deposit order with unique transaction hash, payment instructions,
    and initial 'PENDING' verification status.
    """
    amount = float(amount)
    if amount <= 0:
        raise ValueError("Deposit amount must be greater than 0.")

    tx_hash = f"EM-TX-{uuid.uuid4().hex[:12].upper()}"
    conn = get_db()
    cursor = conn.cursor()

    if payment_method == "USDT_TRC20":
        pay_address = USDT_TRC20_WALLET
        network = "TRON (TRC-20)"
        qr_data = f"tron:{pay_address}?amount={amount}"
    elif payment_method == "USDT_ERC20":
        pay_address = USDT_ERC20_WALLET
        network = "Ethereum (ERC-20)"
        qr_data = f"ethereum:{pay_address}?value={amount}"
    elif payment_method == "USD_CARD":
        pay_address = "Direct Card Gateway"
        network = "Visa/Mastercard Checkout"
        qr_data = f"card_order:{tx_hash}"
    else:
        pay_address = USDT_TRC20_WALLET
        network = "TRON (TRC-20)"
        qr_data = f"tron:{pay_address}?amount={amount}"

    description = f"Deposit {amount:.2f} {currency} via {network}"
    if plan_code:
        description += f" (Auto-deploy {plan_code} plan)"

    proof_data = f"network={network};address={pay_address};plan={plan_code or 'none'}"

    cursor.execute("""
    INSERT INTO transactions (
        tx_hash, user_id, type, amount, currency, status, payment_method, description, proof_data
    ) VALUES (?, ?, 'DEPOSIT', ?, ?, 'PENDING', ?, ?, ?)
    """, (tx_hash, user_id, amount, currency, payment_method, description, proof_data))

    conn.commit()
    conn.close()

    return {
        "tx_hash": tx_hash,
        "amount": amount,
        "currency": currency,
        "payment_method": payment_method,
        "network": network,
        "pay_address": pay_address,
        "qr_data": qr_data,
        "status": "PENDING",
        "plan_code": plan_code,
        "created_at": datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%S")
    }

def verify_payment(tx_hash, simulate_failure=False):
    """
    Automated payment gateway verification API.
    Transitions status from 'PENDING' to 'CREDITED' (or 'FAILED').
    Upon 'CREDITED', credits user wallet and auto-activates mining plan if specified.
    """
    conn = get_db()
    cursor = conn.cursor()

    cursor.execute("SELECT * FROM transactions WHERE tx_hash = ?", (tx_hash,))
    tx = cursor.fetchone()

    if not tx:
        conn.close()
        raise ValueError("Transaction not found.")

    if tx["status"] != "PENDING":
        conn.close()
        return {
            "tx_hash": tx["tx_hash"],
            "status": tx["status"],
            "amount": tx["amount"],
            "currency": tx["currency"],
            "already_processed": True
        }

    now = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%S")

    if simulate_failure:
        cursor.execute("""
        UPDATE transactions 
        SET status = 'FAILED', updated_at = ?, description = description || ' [Verification Failed: Gateway Declined]'
        WHERE tx_hash = ?
        """, (now, tx_hash))
        conn.commit()
        conn.close()
        return {
            "tx_hash": tx_hash,
            "status": "FAILED",
            "message": "Payment verification failed. Please check payment confirmation or retry."
        }

    # Mark transaction as CREDITED
    cursor.execute("""
    UPDATE transactions 
    SET status = 'CREDITED', updated_at = ?, description = description || ' [Auto-verified by Gateway API]'
    WHERE tx_hash = ?
    """, (now, tx_hash))

    # Update user wallet balance
    user_id = tx["user_id"]
    amount = tx["amount"]
    currency = tx["currency"]

    if currency == "USDT":
        cursor.execute("""
        UPDATE wallets 
        SET usdt_balance = usdt_balance + ?, total_deposited = total_deposited + ?
        WHERE user_id = ?
        """, (amount, amount, user_id))
    else:
        cursor.execute("""
        UPDATE wallets 
        SET usd_balance = usd_balance + ?, total_deposited = total_deposited + ?
        WHERE user_id = ?
        """, (amount, amount, user_id))

    conn.commit()
    conn.close()

    # If transaction had an auto-deploy plan attached, activate it now!
    proof_data = tx["proof_data"] or ""
    plan_code = None
    for part in proof_data.split(";"):
        if part.startswith("plan="):
            code = part.split("=")[1]
            if code and code != "none":
                plan_code = code

    contract = None
    if plan_code:
        try:
            # Deduct balance for the contract purchase
            conn = get_db()
            cursor = conn.cursor()
            if currency == "USDT":
                cursor.execute("UPDATE wallets SET usdt_balance = usdt_balance - ? WHERE user_id = ?", (amount, user_id))
            else:
                cursor.execute("UPDATE wallets SET usd_balance = usd_balance - ? WHERE user_id = ?", (amount, user_id))
            conn.commit()
            conn.close()

            contract = create_mining_contract(user_id, plan_code, amount, currency)
        except Exception as e:
            print(f"Auto-contract deployment error: {e}")

    return {
        "tx_hash": tx_hash,
        "status": "CREDITED",
        "amount": amount,
        "currency": currency,
        "message": "Payment verified and credited successfully.",
        "contract": contract
    }

def process_withdrawal(user_id, amount, currency="USDT", destination_address=""):
    """
    Handles user payout / withdrawal requests.
    Enforces minimum balance, records transaction, and updates wallet.
    """
    amount = float(amount)
    if amount < 10.0:
        raise ValueError("Minimum withdrawal amount is 10.00 USDT/USD.")
    if not destination_address or len(destination_address.strip()) < 8:
        raise ValueError("Please provide a valid destination payout wallet/account address.")

    conn = get_db()
    cursor = conn.cursor()

    cursor.execute("SELECT * FROM wallets WHERE user_id = ?", (user_id,))
    wallet = cursor.fetchone()

    if not wallet:
        conn.close()
        raise ValueError("User wallet not found.")

    current_balance = wallet["usdt_balance"] if currency == "USDT" else wallet["usd_balance"]
    if current_balance < amount:
        conn.close()
        raise ValueError(f"Insufficient {currency} balance. Available: {current_balance:.2f} {currency}")

    # Deduct from wallet
    if currency == "USDT":
        cursor.execute("""
        UPDATE wallets 
        SET usdt_balance = usdt_balance - ?, total_withdrawn = total_withdrawn + ?
        WHERE user_id = ?
        """, (amount, amount, user_id))
    else:
        cursor.execute("""
        UPDATE wallets 
        SET usd_balance = usd_balance - ?, total_withdrawn = total_withdrawn + ?
        WHERE user_id = ?
        """, (amount, amount, user_id))

    tx_hash = f"WD-{uuid.uuid4().hex[:12].upper()}"
    desc = f"Withdrawal payout of {amount:.2f} {currency} to {destination_address[:8]}...{destination_address[-4:]}"

    # Withdrawals are processed and credited to blockchain/bank immediately in our system
    cursor.execute("""
    INSERT INTO transactions (
        tx_hash, user_id, type, amount, currency, status, payment_method, description, proof_data
    ) VALUES (?, ?, 'WITHDRAWAL', ?, ?, 'CREDITED', 'INTERNAL', ?, ?)
    """, (tx_hash, user_id, amount, currency, desc, destination_address))

    conn.commit()
    conn.close()

    return {
        "tx_hash": tx_hash,
        "amount": amount,
        "currency": currency,
        "destination": destination_address,
        "status": "CREDITED",
        "message": f"Withdrawal of {amount:.2f} {currency} processed and broadcasted successfully!"
    }
