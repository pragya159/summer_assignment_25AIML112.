import re
from database import get_db

FAQ_KNOWLEDGE_BASE = [
    {
        "keywords": ["contact", "owner", "number", "phone", "email", "call", "whatsapp", "confusion", "doubt", "help", "yashsvi", "talk to human", "agent"],
        "answer": "📞 **Direct Owner Support & Assistance:**\n\nIf you have any doubts, questions, or confusion regarding deposits, plans, or payouts, you can reach out directly to the platform owner for instant one-on-one help:\n\n• **Phone / WhatsApp:** [+91 9452344245](tel:9452344245)\n• **Email:** [yashsvi.singh159@gmail.com](mailto:yashsvi.singh159@gmail.com)\n\nWe ensure all investor queries are resolved promptly!"
    },
    {
        "keywords": ["what is", "e-mining", "cloud mining", "how does it work", "beginner", "explain"],
        "answer": "🚀 **What is E-Mining?**\n\nE-Mining (electronic/cloud mining) allows anyone to participate in cryptocurrency mining without purchasing expensive, loud ASIC hardware or paying high residential electricity bills.\n\nHere is how it works:\n1. **Choose a Mining Plan**: Select your investment amount ($50 to $100,000+) and target hashrate.\n2. **We Run the Hardware**: Our hydro-powered industrial data centers run top-tier Antminer S21 rigs on your behalf.\n3. **Daily Automated Payouts**: The Bitcoin/crypto rewards produced by your allocated hashrate are deposited into your account wallet daily."
    },
    {
        "keywords": ["fee", "2%", "platform fee", "cost", "charge", "commission"],
        "answer": "💎 **Transparent 2% Platform Fee:**\n\nOur platform operates on a performance-aligned model:\n• We take a **2.0% fee exclusively on your mining profit**, NEVER on your original principal investment.\n• **98% of all gross mining yields** are directly credited to your user wallet.\n• This 2% platform fee covers data center technician maintenance, liquid immersion cooling, and 24/7 security monitoring."
    },
    {
        "keywords": ["deposit", "usdt", "usd", "payment", "how to pay", "pay on website", "trc20", "erc20", "card"],
        "answer": "💳 **Deposit & Payment System:**\n\nWe offer seamless 'Pay on Website' options:\n• **USDT (TRC-20 & ERC-20)**: Instant QR code and wallet address. Fast, low network gas fees.\n• **USD Card & Banking**: Direct Visa/Mastercard checkout right on the website.\n• **Auto-Verification**: Once submitted, our gateway auto-detects the blockchain confirmation, immediately changing status to **Credited** and activating your hashrate within seconds!\n\nIf you face any confusion during payment, call the owner directly at **+91 9452344245**."
    },
    {
        "keywords": ["verify", "verification", "credited", "failed", "pending", "status"],
        "answer": "⚡ **Payment Verification Statuses:**\n\n• **Pending**: Your deposit is awaiting network broadcast confirmation.\n• **Credited**: The payment gateway has auto-verified your transaction! Funds are credited to your wallet, and your chosen mining contract begins generating hash power.\n• **Failed**: If network timeout or incorrect amount occurs, the transaction is flagged as Failed so you can retry safely."
    },
    {
        "keywords": ["roi", "calculator", "profit", "return", "earnings", "yield"],
        "answer": "📈 **How the ROI Calculator Works:**\n\nOur interactive ROI tool lets you adjust your investment amount and contract duration (30 to 365 days). It dynamically calculates:\n• **Daily, Weekly, and Monthly Gross Profit** based on network difficulty.\n• **Platform 2% Fee Deduction** clearly itemized.\n• **Net Total Return** that you can withdraw at any time.\n\nTry our ROI Calculator on the homepage to simulate your returns!"
    },
    {
        "keywords": ["withdraw", "withdrawal", "cash out", "payout", "minimum"],
        "answer": "💸 **Withdrawal & Payout Rules:**\n\n• Minimum withdrawal is just **10.00 USDT or USD**.\n• Payouts are processed instantly with zero hidden withdrawal penalties.\n• You can withdraw accrued profits daily or let them compound to buy more hash power."
    },
    {
        "keywords": ["hardware", "rigs", "asic", "datacenter", "energy", "green"],
        "answer": "⚡ **Hardware & Green Energy Infrastructure:**\n\n• We operate fleets of **Bitmain Antminer S21 Pro Hydro (335 TH/s)** and **MicroBT Whatsminer M50S+**.\n• 100% powered by renewable hydro-electric and solar grids, keeping operational costs low and mining sustainable.\n• 99.98% uptime guaranteed by redundant power supply systems."
    },
    {
        "keywords": ["security", "safe", "legit", "escrow", "protection", "hack"],
        "answer": "🛡️ **Institutional Security & Data Protection:**\n\n• User credentials are protected with PBKDF2 cryptographic hashing and salted tokens.\n• 95% of platform digital assets are held in institutional multi-signature cold storage vaults (Ledger Enterprise / Fireblocks).\n• Full SSL 256-bit encryption for all data transactions."
    }
]

def get_chatbot_response(message, user_id=None):
    """
    Intelligent response matcher for support queries.
    """
    cleaned = message.lower().strip()
    
    # Check for direct greetings
    if cleaned in ["hi", "hello", "hey", "start", "greetings"]:
        return {
            "reply": "Hello! 👋 Welcome to **BitForge E-Mining** support. I can assist you with:\n\n• How cloud mining works for beginners\n• Making USDT/USD deposits & payment verification\n• Understanding your ROI and the transparent 2% platform fee\n• Withdrawal requests and wallet balance\n\n📞 **Need direct human assistance?**\nCall or WhatsApp our Owner: **+91 9452344245**\nEmail: **yashsvi.singh159@gmail.com**\n\nWhat can I help you with today?",
            "suggestions": ["Contact Owner", "How does e-mining work?", "What is the 2% fee?", "How to deposit USDT?"]
        }

    # Match against FAQ knowledge base
    best_match = None
    max_hits = 0

    for item in FAQ_KNOWLEDGE_BASE:
        hits = sum(1 for kw in item["keywords"] if re.search(r'\b' + re.escape(kw) + r'\b', cleaned))
        if hits > max_hits:
            max_hits = hits
            best_match = item

    if best_match and max_hits > 0:
        return {
            "reply": best_match["answer"],
            "suggestions": ["Contact Owner", "View Mining Plans", "Open ROI Calculator", "Deposit Funds"]
        }

    # Fallback response
    return {
        "reply": "Thanks for your question! Here are the core details:\n\nBitForge provides institutional e-mining starting from $50 with daily automated payouts, USDT/USD deposits with auto-verification ('Credited' status), and a fair **2% platform fee on profits**.\n\n📞 **Have any confusion or need direct help?**\nContact the owner directly:\n• **Phone / WhatsApp:** [+91 9452344245](tel:9452344245)\n• **Email:** [yashsvi.singh159@gmail.com](mailto:yashsvi.singh159@gmail.com)\n\nYou can also submit a priority ticket in our **Customer Support** section!",
        "suggestions": ["Contact Owner", "How does e-mining work?", "What is the 2% fee?", "Open Support Ticket"]
    }
