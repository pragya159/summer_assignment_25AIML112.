import os
import sys
from database import init_db
from app import app

if __name__ == "__main__":
    print("=" * 60)
    print("  BitForge E-Mining Investment Platform")
    print("=" * 60)
    print("1. Initializing database schema and plans...")
    init_db()
    
    port = int(os.environ.get("PORT", 5000))
    host = "0.0.0.0"
    print(f"2. Platform running at: http://127.0.0.1:{port}")
    print("3. Press Ctrl+C to stop the server.")
    print("=" * 60)
    
    app.run(host=host, port=port, debug=False)
