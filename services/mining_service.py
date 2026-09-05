import uuid
from datetime import datetime, timedelta, timezone
from database import get_db

PLATFORM_FEE_RATE = 0.02  # 2% platform fee on all generated mining profits

def calculate_roi(amount, plan_code="starter", custom_duration=None):
    """
    Computes projected returns for a given investment amount and plan/duration.
    Explicitly breaks down gross profit, 2% platform fee, and net user profit.
    """
    conn = get_db()
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM mining_plans WHERE plan_code = ?", (plan_code,))
    plan = cursor.fetchone()
    conn.close()

    if not plan:
        # Fallback defaults if plan_code is custom
        daily_roi_pct = 1.6
        duration_days = custom_duration or 30
        hashrate_per_dollar = 0.15
        plan_name = "Custom Mining Hash"
    else:
        daily_roi_pct = plan["daily_roi_pct"]
        duration_days = custom_duration or plan["duration_days"]
        hashrate_per_dollar = plan["hashrate_per_dollar"]
        plan_name = plan["name"]

    amount = float(amount)
    hashrate_ths = round(amount * hashrate_per_dollar, 2)

    # Gross calculations
    gross_daily_profit = amount * (daily_roi_pct / 100.0)
    platform_daily_fee = gross_daily_profit * PLATFORM_FEE_RATE
    net_daily_profit = gross_daily_profit - platform_daily_fee

    gross_weekly_profit = gross_daily_profit * 7
    platform_weekly_fee = gross_weekly_profit * PLATFORM_FEE_RATE
    net_weekly_profit = gross_weekly_profit - platform_weekly_fee

    gross_monthly_profit = gross_daily_profit * 30
    platform_monthly_fee = gross_monthly_profit * PLATFORM_FEE_RATE
    net_monthly_profit = gross_monthly_profit - platform_monthly_fee

    gross_total_profit = gross_daily_profit * duration_days
    platform_total_fee = gross_total_profit * PLATFORM_FEE_RATE
    net_total_profit = gross_total_profit - platform_total_fee

    total_net_return = amount + net_total_profit
    net_roi_percentage = (net_total_profit / amount) * 100.0 if amount > 0 else 0

    # Generate projection chart points (up to 12 intervals)
    steps = min(12, duration_days)
    chart_labels = []
    chart_gross = []
    chart_net = []
    chart_principal = []

    step_days = max(1, duration_days // steps)
    for step in range(0, duration_days + 1, step_days):
        chart_labels.append(f"Day {step}")
        chart_principal.append(round(amount, 2))
        step_gross = amount + (gross_daily_profit * step)
        step_net = amount + (net_daily_profit * step)
        chart_gross.append(round(step_gross, 2))
        chart_net.append(round(step_net, 2))

    return {
        "plan_code": plan_code,
        "plan_name": plan_name,
        "investment_amount": round(amount, 2),
        "hashrate_ths": hashrate_ths,
        "duration_days": duration_days,
        "daily_roi_pct": daily_roi_pct,
        "platform_fee_pct": PLATFORM_FEE_RATE * 100,  # 2.0%
        "daily": {
            "gross": round(gross_daily_profit, 4),
            "platform_fee": round(platform_daily_fee, 4),
            "net": round(net_daily_profit, 4)
        },
        "weekly": {
            "gross": round(gross_weekly_profit, 4),
            "platform_fee": round(platform_weekly_fee, 4),
            "net": round(net_weekly_profit, 4)
        },
        "monthly": {
            "gross": round(gross_monthly_profit, 4),
            "platform_fee": round(platform_monthly_fee, 4),
            "net": round(net_monthly_profit, 4)
        },
        "total": {
            "gross_profit": round(gross_total_profit, 2),
            "platform_fee": round(platform_total_fee, 2),
            "net_profit": round(net_total_profit, 2),
            "total_return": round(total_net_return, 2),
            "net_roi_pct": round(net_roi_percentage, 2)
        },
        "chart": {
            "labels": chart_labels,
            "principal": chart_principal,
            "gross": chart_gross,
            "net": chart_net
        }
    }

def create_mining_contract(user_id, plan_code, amount, currency="USDT"):
    """
    Activates a new mining contract for a user.
    """
    conn = get_db()
    cursor = conn.cursor()

    cursor.execute("SELECT * FROM mining_plans WHERE plan_code = ?", (plan_code,))
    plan = cursor.fetchone()
    if not plan:
        conn.close()
        raise ValueError(f"Invalid mining plan: {plan_code}")

    amount = float(amount)
    if amount < plan["min_investment"] or amount > plan["max_investment"]:
        conn.close()
        raise ValueError(
            f"Investment for {plan['name']} must be between ${plan['min_investment']:,.0f} and ${plan['max_investment']:,.0f}"
        )

    hashrate = round(amount * plan["hashrate_per_dollar"], 2)
    daily_profit = round(amount * (plan["daily_roi_pct"] / 100.0), 4)
    total_projected = round(daily_profit * plan["duration_days"], 2)

    now = datetime.now(timezone.utc).replace(tzinfo=None)
    end_date = now + timedelta(days=plan["duration_days"])
    contract_id = f"CT-{uuid.uuid4().hex[:8].upper()}"

    cursor.execute("""
    INSERT INTO contracts (
        contract_id, user_id, plan_code, plan_name, amount, currency,
        hashrate_ths, daily_roi_pct, daily_profit, total_projected_profit,
        start_date, end_date, status, last_profit_calc
    ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, 'ACTIVE', ?)
    """, (
        contract_id, user_id, plan_code, plan["name"], amount, currency,
        hashrate, plan["daily_roi_pct"], daily_profit, total_projected,
        now.strftime("%Y-%m-%d %H:%M:%S"), end_date.strftime("%Y-%m-%d %H:%M:%S"),
        now.strftime("%Y-%m-%d %H:%M:%S")
    ))

    # Update active hashrate in user's wallet
    cursor.execute("""
    UPDATE wallets SET active_hashrate = active_hashrate + ? WHERE user_id = ?
    """, (hashrate, user_id))

    conn.commit()
    conn.close()

    return {
        "contract_id": contract_id,
        "plan_name": plan["name"],
        "amount": amount,
        "currency": currency,
        "hashrate_ths": hashrate,
        "daily_roi_pct": plan["daily_roi_pct"],
        "daily_profit": daily_profit,
        "duration_days": plan["duration_days"],
        "end_date": end_date.strftime("%Y-%m-%d"),
        "status": "ACTIVE"
    }

def accrue_profits(user_id):
    """
    Checks active mining contracts, calculates accrued profit based on elapsed time,
    deducts 2% platform fee, records fee in platform_revenue, and credits net profit to wallet.
    """
    conn = get_db()
    cursor = conn.cursor()

    cursor.execute("""
    SELECT * FROM contracts WHERE user_id = ? AND status = 'ACTIVE'
    """, (user_id,))
    contracts = cursor.fetchall()

    if not contracts:
        conn.close()
        return {"accrued": 0.0, "fee": 0.0, "net": 0.0, "count": 0}

    now = datetime.now(timezone.utc).replace(tzinfo=None)
    total_gross = 0.0
    total_fee = 0.0
    total_net = 0.0

    for contract in contracts:
        last_calc = datetime.strptime(contract["last_profit_calc"], "%Y-%m-%d %H:%M:%S")
        end_date = datetime.strptime(contract["end_date"], "%Y-%m-%d %H:%M:%S")

        calc_until = min(now, end_date)
        elapsed_seconds = (calc_until - last_calc).total_seconds()

        # Minimum 5 seconds between accruals to avoid micro-transactions spam
        if elapsed_seconds < 5:
            continue

        # Profit per second = daily_profit / 86400
        seconds_in_day = 86400.0
        gross_profit = (contract["daily_profit"] / seconds_in_day) * elapsed_seconds
        if gross_profit <= 0:
            continue

        fee_amount = gross_profit * PLATFORM_FEE_RATE
        net_profit = gross_profit - fee_amount

        total_gross += gross_profit
        total_fee += fee_amount
        total_net += net_profit

        # Record 2% platform revenue
        cursor.execute("""
        INSERT INTO platform_revenue (
            user_id, contract_id, gross_profit, fee_rate, fee_amount, net_user_profit, currency
        ) VALUES (?, ?, ?, ?, ?, ?, ?)
        """, (
            user_id, contract["contract_id"], gross_profit, PLATFORM_FEE_RATE,
            fee_amount, net_profit, contract["currency"]
        ))

        # Update contract status & last_profit_calc
        new_status = 'COMPLETED' if now >= end_date else 'ACTIVE'
        cursor.execute("""
        UPDATE contracts 
        SET total_claimed_profit = total_claimed_profit + ?,
            last_profit_calc = ?,
            status = ?
        WHERE contract_id = ?
        """, (
            net_profit,
            calc_until.strftime("%Y-%m-%d %H:%M:%S"),
            new_status,
            contract["contract_id"]
        ))

        # If completed, reduce active hashrate
        if new_status == 'COMPLETED':
            cursor.execute("""
            UPDATE wallets SET active_hashrate = MAX(0.0, active_hashrate - ?) WHERE user_id = ?
            """, (contract["hashrate_ths"], user_id))

    if total_net > 0:
        # Credit wallet
        cursor.execute("""
        UPDATE wallets 
        SET usdt_balance = usdt_balance + ?,
            total_profit = total_profit + ?
        WHERE user_id = ?
        """, (total_net, total_net, user_id))

        # Record payout transaction
        tx_hash = f"0x{uuid.uuid4().hex}"
        cursor.execute("""
        INSERT INTO transactions (
            tx_hash, user_id, type, amount, currency, status, payment_method, description
        ) VALUES (?, ?, 'PROFIT_PAYOUT', ?, 'USDT', 'CREDITED', 'INTERNAL', ?)
        """, (
            tx_hash, user_id, total_net,
            f"Automated Mining Yield credited (2% platform fee: ${total_fee:.4f} deducted)"
        ))

    conn.commit()
    conn.close()

    return {
        "accrued_gross": round(total_gross, 6),
        "platform_fee": round(total_fee, 6),
        "net_profit": round(total_net, 6),
        "contracts_evaluated": len(contracts)
    }
