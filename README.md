# 🏧 ATM Management System – C++ Console Application

Welcome to the **ATM Management System** – a simple yet functional console-based banking system developed using **C++**. This project simulates ATM-like operations including account creation, login, balance checks, deposits, and withdrawals using file handling.

---

## 📌 Features

✨ **Create Account**  
🔑 **Login / Logout**  
🔁 **Reset PIN / Forgot PIN Recovery**  
💰 **Deposit Money**  
💸 **Withdraw Money**  
📊 **Check Account Balance**

---

## 🛠️ Technologies Used

- **C++** (Standard Library)
- **File Handling** (`fstream`)
- **Input Validation** (`std::limits`, `cin.ignore()`)
- **Random ID Generation** (`rand`, `time`)
- **Modular Programming** (functions for clarity and reuse)

---

## 🚀 How It Works

1. **New User?**
   - Use `Create Account` to register.
   - You'll receive a unique 6-digit account ID and set a 4-digit PIN.
   - A **security phrase** is required for PIN recovery.

2. **Returning User?**
   - Login with your ID and PIN.
   - Access account operations: deposit, withdraw, or check balance.

3. **Forgot Your PIN?**
   - Recover it using your username and security phrase.

4. **Security First:**
   - Data is saved securely in `account.txt`.
   - Updates are handled using temporary files for safety.

---
💡 Future Enhancements
🔐 Encryption for account data

🌐 GUI version using Qt or SFML

🧪 Unit testing integration

📈 Transaction history tracking

## 🧪 Sample Flow

```bash
> Create new account
> Generate new PIN
> Login to account
> [Account Menu]
    1. Deposit money
    2. Check balance
    3. Withdraw money
    4. Logout
