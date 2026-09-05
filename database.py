import sqlite3
import os
from datetime import datetime, timedelta

DB_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "emining.db")

def get_db():
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    return conn

def init_db():
    conn = get_db()
    cursor = conn.cursor()

    # Users table
    cursor.execute("""
    CREATE TABLE IF NOT EXISTS users (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        user_id TEXT UNIQUE NOT NULL,
        username TEXT UNIQUE NOT NULL,
        email TEXT UNIQUE NOT NULL,
        password_hash TEXT NOT NULL,
        role TEXT DEFAULT 'INVESTOR',
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );
    """)

    # User Wallets
    cursor.execute("""
    CREATE TABLE IF NOT EXISTS wallets (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        user_id TEXT UNIQUE NOT NULL,
        usdt_balance REAL DEFAULT 0.0,
        usd_balance REAL DEFAULT 0.0,
        total_deposited REAL DEFAULT 0.0,
        total_withdrawn REAL DEFAULT 0.0,
        total_profit REAL DEFAULT 0.0,
        active_hashrate REAL DEFAULT 0.0,
        FOREIGN KEY (user_id) REFERENCES users (user_id)
    );
    """)

    # Mining Plans
    cursor.execute("""
    CREATE TABLE IF NOT EXISTS mining_plans (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        plan_code TEXT UNIQUE NOT NULL,
        name TEXT NOT NULL,
        tier TEXT NOT NULL,
        min_investment REAL NOT NULL,
        max_investment REAL NOT NULL,
        daily_roi_pct REAL NOT NULL,
        duration_days INTEGER NOT NULL,
        hashrate_per_dollar REAL NOT NULL,
        description TEXT NOT NULL,
        hardware TEXT NOT NULL
    );
    """)

    # Active Mining Contracts
    cursor.execute("""
    CREATE TABLE IF NOT EXISTS contracts (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        contract_id TEXT UNIQUE NOT NULL,
        user_id TEXT NOT NULL,
        plan_code TEXT NOT NULL,
        plan_name TEXT NOT NULL,
        amount REAL NOT NULL,
        currency TEXT NOT NULL DEFAULT 'USDT',
        hashrate_ths REAL NOT NULL,
        daily_roi_pct REAL NOT NULL,
        daily_profit REAL NOT NULL,
        total_projected_profit REAL NOT NULL,
        total_claimed_profit REAL DEFAULT 0.0,
        start_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        end_date TIMESTAMP NOT NULL,
        status TEXT DEFAULT 'ACTIVE',
        last_profit_calc TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        FOREIGN KEY (user_id) REFERENCES users (user_id)
    );
    """)

    # Transactions & Payment Ledger
    cursor.execute("""
    CREATE TABLE IF NOT EXISTS transactions (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        tx_hash TEXT UNIQUE NOT NULL,
        user_id TEXT NOT NULL,
        type TEXT NOT NULL,
        amount REAL NOT NULL,
        currency TEXT NOT NULL,
        status TEXT NOT NULL,
        payment_method TEXT NOT NULL,
        description TEXT,
        proof_data TEXT,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        FOREIGN KEY (user_id) REFERENCES users (user_id)
    );
    """)

    # Platform Revenue (Transparent 2% fee tracking on all profits)
    cursor.execute("""
    CREATE TABLE IF NOT EXISTS platform_revenue (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        user_id TEXT NOT NULL,
        contract_id TEXT,
        gross_profit REAL NOT NULL,
        fee_rate REAL DEFAULT 0.02,
        fee_amount REAL NOT NULL,
        net_user_profit REAL NOT NULL,
        currency TEXT NOT NULL,
        timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        FOREIGN KEY (user_id) REFERENCES users (user_id)
    );
    """)

    # Support Tickets
    cursor.execute("""
    CREATE TABLE IF NOT EXISTS support_tickets (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        ticket_id TEXT UNIQUE NOT NULL,
        user_id TEXT NOT NULL,
        subject TEXT NOT NULL,
        category TEXT NOT NULL,
        message TEXT NOT NULL,
        status TEXT DEFAULT 'OPEN',
        priority TEXT DEFAULT 'MEDIUM',
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        FOREIGN KEY (user_id) REFERENCES users (user_id)
    );
    """)

    # Seed Default Mining Plans if not present
    cursor.execute("SELECT COUNT(*) FROM mining_plans")
    if cursor.fetchone()[0] == 0:
        plans = [
            (
                "starter",
                "Starter Hash",
                "Bronze",
                50.0,
                499.0,
                1.4,
                30,
                0.12,
                "Ideal for beginners exploring cloud crypto mining with instant daily payouts.",
                "Antminer S19k Pro (120 TH/s node slice)"
            ),
            (
                "advanced",
                "Advanced Node",
                "Silver",
                500.0,
                2499.0,
                1.8,
                90,
                0.15,
                "High-performance rig allocation with low latency and enhanced power efficiency.",
                "Whatsminer M50S+ (136 TH/s dedicated)"
            ),
            (
                "enterprise",
                "Enterprise Hydro",
                "Gold",
                2500.0,
                9999.0,
                2.3,
                180,
                0.18,
                "Industrial liquid-cooled mining containers powered by 100% renewable hydroelectricity.",
                "Antminer S21 Hydro (335 TH/s hydro cluster)"
            ),
            (
                "whale",
                "Whale Mega-Farm",
                "Diamond",
                10000.0,
                100000.0,
                2.8,
                365,
                0.22,
                "Institutional colocation cluster with maximum hashrate yield and dedicated account manager.",
                "Multi-Pod Megawatt ASIC Fleet (Custom OC)"
            )
        ]
        cursor.executemany("""
        INSERT INTO mining_plans (
            plan_code, name, tier, min_investment, max_investment, 
            daily_roi_pct, duration_days, hashrate_per_dollar, description, hardware
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        """, plans)

    conn.commit()
    conn.close()

if __name__ == "__main__":
    init_db()
    print("Database initialized successfully with schema and seeded plans.")
