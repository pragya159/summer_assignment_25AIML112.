# BitForge — E-Mining (Crypto Mining Investment) Platform

A modern, institutional-grade cryptocurrency mining investment web platform designed for crypto newcomers and experienced investors alike. The platform simplifies cloud mining by eliminating physical hardware hassles, providing an interactive ROI calculator, supporting auto-verified USDT/USD payments, automating profit distribution with a transparent 2% platform fee, and offering customer support via an integrated AI chatbot.

---

## 🌟 Key Features

### 1. Homepage & Beginner Explainer
- **What is E-Mining**: Clear, non-technical explanation of cloud/electronic mining.
- **3-Step Walkthrough**: Choose Hashrate $\rightarrow$ Pay on Website $\rightarrow$ Daily Automated Yields.
- **Crypto Primer / Glossary**: Plain-English explanations of Hashrate (TH/s), Proof-of-Work, and USDT vs USD.
- **Live Network Telemetry**: Global hashrate (1.42 EH/s), active miners, and uptime metrics.

### 2. Benefits Section
- **Zero Hardware Maintenance**: No burnt boards, loud fans, or heat dissipation issues.
- **100% Green Energy**: Hydro and solar co-location in low-power-cost regions.
- **Instant Daily Payouts**: Automated crediting directly to user wallet balance.
- **Cold Storage Escrow**: Multi-signature vault protection for platform liquidity.
- **Transparent 2% Profit Fee**: We only earn when you earn (deducted from yields, never principal).
- **Multi-Currency Freedom**: USDT (TRC-20 / ERC-20) and USD Card payments.

### 3. Interactive ROI Calculator
- Dynamic sliders and preset buttons ($100, $500, $1,000, $2,500, $10,000).
- Flexible duration selection (30, 90, 180, 365 days).
- **Real-Time Breakdown**:
  - Daily, Weekly, and Monthly Gross & Net Yields.
  - **Platform 2% Fee Deduction** clearly itemized.
  - Net User Profit and Total Return calculation.
  - Interactive **Chart.js** projection curve showing profit trajectory.
- One-click CTA to immediately pre-fill checkout for the selected amount.

### 4. User Account System
- Secure user registration with unique User ID generation (e.g. `EM-58291`).
- Strong password encryption using salted **PBKDF2-HMAC-SHA256**.
- JWT / session token authentication protecting all user endpoints.
- **Wallet Dashboard**:
  - Live USDT and USD balance tracker.
  - Active Mining Hashrate counter (TH/s).
  - Cumulative Net Profit and Total Platform 2% Fee Paid.
  - Real-time animated satoshi profit ticker.
  - Active Mining Contracts table with live yield rates.
  - Payment and payout transaction ledger with status badges (`CREDITED`, `PENDING`, `FAILED`).

### 5. Payment System & "Pay on Website" Checkout
- **Direct On-Site Checkout**:
  - **USDT (TRC-20 & ERC-20)**: Generates dynamic QR code and unique deposit address with one-click copy.
  - **USD**: Direct credit/debit card interface.
- **Auto-Verification Gateway API**:
  - Automatically simulates external blockchain network confirmations.
  - Transitions transaction status: `PENDING` $\rightarrow$ `CREDITED` (or `FAILED` on simulated decline).
  - Automatically updates wallet balances and immediately provisions the active mining contract.

### 6. Profit & Payout System
- **Transparent 2% Platform Fee**:
  $$\text{Gross Profit} = \text{Investment} \times \left(\frac{\text{Daily ROI \%}}{100}\right)$$
  $$\text{Platform Fee} = \text{Gross Profit} \times 0.02$$
  $$\text{Net User Profit} = \text{Gross Profit} \times 0.98$$
- Platform fees are logged to the `platform_revenue` database table.
- **Withdrawals / Payouts**:
  - Minimum withdrawal threshold: $10.00 USDT or USD.
  - Instant external wallet/bank address payout simulation.

### 7. Customer Support & AI Chatbot
- **Interactive FAQ Accordion**: Addresses common questions regarding withdrawals, security, and the 2% fee.
- **Support Ticket Helpdesk**: Create priority inquiry tickets categorized by department.
- **Floating AI Chatbot**:
  - Available on every page.
  - Instant answers to questions about e-mining, payments, 2% fee, and ROI.
  - Quick action suggestion chips.

---

## 🛠️ Tech Stack

- **Backend**: Python 3.12, Flask, Flask-CORS, Werkzeug Security.
- **Database**: SQLite3 with auto-initialization and parameterized queries.
- **Frontend**: HTML5, Modern CSS3 with Glassmorphism, Vanilla JS, Chart.js.
- **Architecture**: Modular services pattern (`services/mining_service.py`, `services/payment_service.py`, `services/chatbot_service.py`).

---

## 🚀 How to Run

1. Open PowerShell and navigate to the project directory:
   ```powershell
   cd C:\Users\pragy\gitdemo
   ```

2. Start the web server using the virtual environment:
   ```powershell
   .\.venv\Scripts\python.exe run.py
   ```

3. Open your browser and navigate to:
   ```
   http://127.0.0.1:5000
   ```

---

## 🧪 Running Automated Tests

Run the test suite to verify the entire platform lifecycle:
```powershell
.\.venv\Scripts\python.exe -m unittest tests/test_platform.py
```

## Deploy to Google Cloud Run

Install the Google Cloud CLI, authenticate, create/select a Google Cloud project,
enable billing, and run these commands from `C:\Users\pragy\gitdemo`:

```powershell
gcloud auth login
gcloud config set project YOUR_PROJECT_ID
gcloud services enable run.googleapis.com cloudbuild.googleapis.com
gcloud run deploy bitforge `
  --source . `
  --region asia-south1 `
  --allow-unauthenticated
```

Cloud Run will build the Docker image and return a public HTTPS URL.
