import os
import unittest
import json
import time

# Ensure clean test database
os.environ["SECRET_KEY"] = "test-secret-key-12345"

from app import app
from database import init_db, get_db

class TestEMiningPlatform(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        init_db()

    def setUp(self):
        self.app = app.test_client()
        self.app.testing = True

    def test_01_roi_calculator_and_platform_fee(self):
        """Verify ROI calculation and exact 2% platform fee deduction"""
        res = self.app.post("/api/calculator/calculate", json={
            "amount": 1000.0,
            "plan_code": "advanced", # 1.8% daily, 90 days
            "duration": 90
        })
        self.assertEqual(res.status_code, 200)
        data = res.get_json()["data"]
        
        # Gross daily: 1000 * 0.018 = 18.0
        gross_daily = data["daily"]["gross"]
        self.assertAlmostEqual(gross_daily, 18.0, places=2)
        
        # 2% fee on gross: 18.0 * 0.02 = 0.36
        fee_daily = data["daily"]["platform_fee"]
        self.assertAlmostEqual(fee_daily, 0.36, places=2)
        
        # Net daily: 18.0 - 0.36 = 17.64
        net_daily = data["daily"]["net"]
        self.assertAlmostEqual(net_daily, 17.64, places=2)
        
        # Total duration 90 days: gross total = 18 * 90 = 1620.0
        self.assertAlmostEqual(data["total"]["gross_profit"], 1620.0, places=2)
        # 2% fee = 1620 * 0.02 = 32.4
        self.assertAlmostEqual(data["total"]["platform_fee"], 32.4, places=2)
        # Net total profit = 1620 - 32.4 = 1587.6
        self.assertAlmostEqual(data["total"]["net_profit"], 1587.6, places=2)

    def test_02_user_registration_and_login(self):
        """Verify user account creation, password hashing, and login"""
        username = f"miner_test_{int(time.time())}"
        email = f"{username}@example.com"
        password = "SecurePassword#2026"

        reg_res = self.app.post("/api/auth/register", json={
            "username": username,
            "email": email,
            "password": password
        })
        self.assertEqual(reg_res.status_code, 201)
        reg_data = reg_res.get_json()
        self.assertTrue(reg_data["success"])
        self.assertIn("token", reg_data)
        user_id = reg_data["user"]["user_id"]
        self.assertTrue(user_id.startswith("EM-"))

        # Verify password is hash-stored, not plaintext
        conn = get_db()
        cursor = conn.cursor()
        cursor.execute("SELECT password_hash FROM users WHERE user_id = ?", (user_id,))
        stored_hash = cursor.fetchone()[0]
        conn.close()
        self.assertNotEqual(stored_hash, password)
        self.assertTrue(stored_hash.startswith("scrypt:") or stored_hash.startswith("pbkdf2:"))

        # Verify login
        login_res = self.app.post("/api/auth/login", json={
            "login": username,
            "password": password
        })
        self.assertEqual(login_res.status_code, 200)
        login_data = login_res.get_json()
        self.assertTrue(login_data["success"])
        token = login_data["token"]

        # Verify authenticated endpoint access
        me_res = self.app.get("/api/auth/me", headers={"Authorization": f"Bearer {token}"})
        self.assertEqual(me_res.status_code, 200)
        self.assertEqual(me_res.get_json()["user"]["username"], username)

    def test_03_payment_order_and_auto_verification(self):
        """Verify USDT/USD checkout flow, PENDING status, and auto-verification to CREDITED"""
        # Register user
        username = f"pay_test_{int(time.time())}"
        reg_res = self.app.post("/api/auth/register", json={
            "username": username,
            "email": f"{username}@example.com",
            "password": "Password123!"
        })
        token = reg_res.get_json()["token"]
        headers = {"Authorization": f"Bearer {token}"}

        # 1. Create checkout deposit order for USDT TRC20 with 'starter' plan
        order_res = self.app.post("/api/payments/checkout", headers=headers, json={
            "amount": 250.0,
            "currency": "USDT",
            "payment_method": "USDT_TRC20",
            "plan_code": "starter"
        })
        self.assertEqual(order_res.status_code, 200)
        order = order_res.get_json()["order"]
        tx_hash = order["tx_hash"]
        self.assertEqual(order["status"], "PENDING")
        self.assertEqual(order["amount"], 250.0)
        self.assertIn("TRX", order["pay_address"])

        # 2. Verify payment via Gateway API
        verify_res = self.app.post("/api/payments/verify", headers=headers, json={
            "tx_hash": tx_hash,
            "simulate_fail": False
        })
        self.assertEqual(verify_res.status_code, 200)
        result = verify_res.get_json()["result"]
        self.assertEqual(result["status"], "CREDITED")

        # 3. Check wallet has active contract deployed and hashrate updated
        wallet_res = self.app.get("/api/wallet/summary", headers=headers)
        self.assertEqual(wallet_res.status_code, 200)
        wallet = wallet_res.get_json()["wallet"]
        self.assertGreater(wallet["active_hashrate_ths"], 0)
        self.assertEqual(wallet["active_contracts_count"], 1)

        # 4. Check active contracts list
        contracts_res = self.app.get("/api/contracts/active", headers=headers)
        contracts = contracts_res.get_json()["contracts"]
        self.assertEqual(len(contracts), 1)
        self.assertEqual(contracts[0]["status"], "ACTIVE")
        self.assertEqual(contracts[0]["amount"], 250.0)

    def test_04_profit_accrual_and_platform_revenue(self):
        """Verify mining yield accrual and 2% fee deduction into platform_revenue table"""
        # Register user
        username = f"profit_test_{int(time.time())}"
        reg_res = self.app.post("/api/auth/register", json={
            "username": username,
            "email": f"{username}@example.com",
            "password": "Password123!"
        })
        token = reg_res.get_json()["token"]
        user_id = reg_res.get_json()["user"]["user_id"]
        headers = {"Authorization": f"Bearer {token}"}

        # Create deposit and auto-credit
        order_res = self.app.post("/api/payments/checkout", headers=headers, json={
            "amount": 1000.0,
            "currency": "USDT",
            "payment_method": "USDT_TRC20",
            "plan_code": "advanced"
        })
        tx_hash = order_res.get_json()["order"]["tx_hash"]
        self.app.post("/api/payments/verify", headers=headers, json={"tx_hash": tx_hash})

        # Simulate time passage by adjusting start_date and last_profit_calc back by 1 day
        conn = get_db()
        cursor = conn.cursor()
        cursor.execute("""
        UPDATE contracts 
        SET last_profit_calc = datetime('now', '-1 day')
        WHERE user_id = ?
        """, (user_id,))
        conn.commit()
        conn.close()

        # Trigger profit accrual
        accrue_res = self.app.post("/api/profits/accrue", headers=headers)
        self.assertEqual(accrue_res.status_code, 200)
        accrual = accrue_res.get_json()["accrual"]
        self.assertGreater(accrual["accrued_gross"], 0)
        self.assertGreater(accrual["platform_fee"], 0)
        self.assertGreater(accrual["net_profit"], 0)

        # Check platform_revenue table recorded the 2% fee
        conn = get_db()
        cursor = conn.cursor()
        cursor.execute("SELECT * FROM platform_revenue WHERE user_id = ?", (user_id,))
        rev = cursor.fetchone()
        conn.close()
        self.assertIsNotNone(rev)
        self.assertAlmostEqual(rev["fee_rate"], 0.02)
        self.assertAlmostEqual(rev["fee_amount"], accrual["platform_fee"], places=3)

    def test_05_chatbot_knowledge_responses(self):
        """Verify AI chatbot responds to core mining questions"""
        res1 = self.app.post("/api/chatbot/message", json={"message": "What is e-mining?"})
        self.assertEqual(res1.status_code, 200)
        data1 = res1.get_json()
        self.assertIn("E-Mining", data1["reply"])

        res2 = self.app.post("/api/chatbot/message", json={"message": "What is the 2% platform fee?"})
        self.assertEqual(res2.status_code, 200)
        data2 = res2.get_json()
        self.assertIn("2.0% fee", data2["reply"])

if __name__ == "__main__":
    unittest.main()
