#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <stdexcept>
#include <string>
#include <limits>
#include <vector>

using namespace std;

// Structure to store user account details
struct Account {
    string id_name;     // Username
    string id;          // Unique 6-digit account ID
    string pin;         // 4-digit PIN
    string info;        // Security phrase for password recovery
    double balance;     // Account balance
};

// Global variables to track current session
Account currentUser;    // Stores currently logged-in user's information
bool isLoggedIn = false;  // Tracks login status

// Helper functions
void clearInputBuffer() {
    // Clears any leftover characters in input buffer to prevent input errors
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool isNumeric(const string& str) {
    // Checks if a string contains only numeric characters
    return !str.empty() && str.find_first_not_of("0123456789") == string::npos;
}

void make_account() {
    // Function to create a new bank account
    // 1. Collects user information
    // 2. Generates random 6-digit account ID
    // 3. Validates 4-digit PIN
    // 4. Stores account info in file with initial balance of 0
    Account newAccount;
    cout << "Create Account\n";
    cout << "---------------\n";
    
    cout << "Enter username: ";
    cin >> newAccount.id_name;
    
    cout << "Enter security phrase (for password recovery): ";
    clearInputBuffer();
    getline(cin, newAccount.info);
    
    // Generate random ID
    newAccount.id = to_string(rand() % 900000 + 100000);  // 6-digit ID
    
    cout << "Create PIN (4 digits): ";
    cin >> newAccount.pin;
    while (newAccount.pin.length() != 4 || !isNumeric(newAccount.pin)) {
        cout << "Invalid PIN. Please enter 4 digits: ";
        cin >> newAccount.pin;
    }

    // ... existing code ...
    ofstream account("account.txt", ios::app);
    if (!account) {
        cerr << "Error: Unable to open account file\n";
        return;
    }
    
    // Write account info in consistent format
    account << newAccount.id_name << "|" << newAccount.id << "|" 
           << newAccount.pin << "|" << newAccount.info << "|0.0\n";
    account.close();

    cout << "\nAccount created successfully!\n";
    cout << "Username: " << newAccount.id_name << "\n";
    cout << "Account ID: " << newAccount.id << "\n";
    cout << "Please keep your ID safe!\n";
}

bool generate_pin() {
    // Function to reset PIN for existing account
    // 1. Requires username and account ID verification
    // 2. Updates PIN in file while preserving other account details
    // 3. Uses temporary file to safely update records
    string id_name, id, pin;
    cout << "Reset PIN\n";
    cout << "---------\n";
    cout << "Enter username: ";
    cin >> id_name;
    cout << "Enter account ID: ";
    cin >> id;

    ifstream readFile("account.txt");
    ofstream tempFile("account_temp.txt");
    
    if (!readFile || !tempFile) {
        cerr << "Error: Unable to process files\n";
        return false;
    }

    bool found = false;
    string line;
    
    while (getline(readFile, line)) {
        size_t pos = 0;
        string delimiter = "|";
        vector<string> tokens;
        
        // Split line into tokens
        while ((pos = line.find(delimiter)) != string::npos) {
            tokens.push_back(line.substr(0, pos));
            line.erase(0, pos + delimiter.length());
        }
        tokens.push_back(line);

        if (tokens[0] == id_name && tokens[1] == id) {
            found = true;
            cout << "Create new PIN (4 digits): ";
            cin >> pin;
            
            while (pin.length() != 4 || !isNumeric(pin)) {
                cout << "Invalid PIN. Please enter 4 digits: ";
                cin >> pin;
            }
            
            // Write updated account info
            tempFile << tokens[0] << "|" << tokens[1] << "|" 
                    << pin << "|" << tokens[3] << "|" << tokens[4] << "\n";
                    
            cout << "\nPIN updated successfully!\n";
            cout << "Username: " << tokens[0] << "\n";
            cout << "Account ID: " << tokens[1] << "\n";
        } else {
            tempFile << line << "\n";
        }
    }

    readFile.close();
    tempFile.close();

    if (found) {
        remove("account.txt");
        rename("account_temp.txt", "account.txt");
        return true;
    }

    remove("account_temp.txt");
    cout << "Error: Account not found\n";
    return false;
}

bool log_in() {
    // Function to authenticate user
    // 1. Verifies account ID and PIN
    // 2. Loads user information into currentUser if successful
    // 3. Sets isLoggedIn flag
    string id, pin;
    cout << "Login\n";
    cout << "-----\n";
    cout << "Enter account ID: ";
    cin >> id;
    cout << "Enter PIN: ";
    cin >> pin;

    ifstream readFile("account.txt");
    if (!readFile) {
        cerr << "Error: Unable to read account file\n";
        return false;
    }

    string line;
    bool found = false;
    
    while (getline(readFile, line)) {
        size_t pos = 0;
        string delimiter = "|";
        vector<string> tokens;
        
        while ((pos = line.find(delimiter)) != string::npos) {
            tokens.push_back(line.substr(0, pos));
            line.erase(0, pos + delimiter.length());
        }
        tokens.push_back(line);

        if (tokens[1] == id && tokens[2] == pin) {
            found = true;
            // Store user information
            currentUser.id_name = tokens[0];
            currentUser.id = tokens[1];
            currentUser.pin = tokens[2];
            currentUser.info = tokens[3];
            currentUser.balance = stod(tokens[4]);
            isLoggedIn = true;
            
            cout << "\nLogin successful!\n";
            cout << "Welcome, " << currentUser.id_name << "!\n";
            readFile.close();
            return true;
        }
    }

    readFile.close();
    cout << "Error: Invalid ID or PIN\n";
    return false;
}

bool forget_pin() {
    string id_name, info;
    cout << "Forgot PIN Recovery\n";
    cout << "-----------------\n";
    cout << "Enter username: ";
    cin >> id_name;
    cout << "Enter security phrase (for password recovery): ";
    clearInputBuffer();
    getline(cin, info);

    ifstream readFile("account.txt");
    ofstream tempFile("account_temp.txt");
    
    if (!readFile || !tempFile) {
        cerr << "Error: Unable to process files\n";
        return false;
    }

    bool found = false;
    string line;
    
    while (getline(readFile, line)) {
        size_t pos = 0;
        string delimiter = "|";
        vector<string> tokens;
        
        // Split line into tokens
        while ((pos = line.find(delimiter)) != string::npos) {
            tokens.push_back(line.substr(0, pos));
            line.erase(0, pos + delimiter.length());
        }
        tokens.push_back(line);

        if (tokens[0] == id_name && tokens[3] == info) {
            found = true;
            string new_pin;
            cout << "Create new PIN (4 digits): ";
            cin >> new_pin;
            
            while (new_pin.length() != 4 || !isNumeric(new_pin)) {
                cout << "Invalid PIN. Please enter 4 digits: ";
                cin >> new_pin;
            }
            
            // Write updated account info
            tempFile << tokens[0] << "|" << tokens[1] << "|" 
                    << new_pin << "|" << tokens[3] << "|" << tokens[4] << "\n";
                    
            cout << "\nPIN reset successful!\n";
            cout << "Username: " << tokens[0] << "\n";
            cout << "Account ID: " << tokens[1] << "\n";
        } else {
            tempFile << line << "\n";
        }
    }

    readFile.close();
    tempFile.close();

    if (found) {
        remove("account.txt");
        rename("account_temp.txt", "account.txt");
        return true;
    }

    remove("account_temp.txt");
    cout << "Error: Invalid username or security phrase\n";
    return false;
}

bool store_amount() {
    // Function to deposit money
    // 1. Requires user to be logged in
    // 2. Validates deposit amount
    // 3. Updates balance in both file and current session
    if (!isLoggedIn) {
        cout << "Error: Please log in first\n";
        return false;
    }

    double amount;
    cout << "Deposit Money\n";
    cout << "-------------\n";
    cout << "Enter amount to deposit: $";
    
    while (!(cin >> amount) || amount <= 0) {
        cout << "Please enter a valid positive amount: $";
        clearInputBuffer();
    }

    // Read account file
    ifstream readFile("account.txt");
    ofstream tempFile("account_temp.txt");
    
    if (!readFile || !tempFile) {
        cerr << "Error: Unable to process files\n";
        return false;
    }

    string line;
    bool found = false;
    
    while (getline(readFile, line)) {
        size_t pos = 0;
        string delimiter = "|";
        vector<string> tokens;
        
        while ((pos = line.find(delimiter)) != string::npos) {
            tokens.push_back(line.substr(0, pos));
            line.erase(0, pos + delimiter.length());
        }
        tokens.push_back(line);

        if (tokens[1] == currentUser.id) {
            found = true;
            double currentBalance = stod(tokens[4]);
            double newBalance = currentBalance + amount;
            
            // Update both file and current user's balance
            tempFile << tokens[0] << "|" << tokens[1] << "|" 
                    << tokens[2] << "|" << tokens[3] << "|" << newBalance << "\n";
            currentUser.balance = newBalance;
                    
            cout << "\nDeposit successful!\n";
            cout << "Previous balance: $" << currentBalance << "\n";
            cout << "Deposited amount: $" << amount << "\n";
            cout << "New balance: $" << newBalance << "\n";
        } else {
            tempFile << line << "\n";
        }
    }

    readFile.close();
    tempFile.close();

    if (found) {
        remove("account.txt");
        rename("account_temp.txt", "account.txt");
        return true;
    }

    remove("account_temp.txt");
    cout << "Error: Account not found\n";
    return false;
}

void check_amount() {
    // Function to display current balance
    // 1. Requires user to be logged in
    // 2. Shows username and current balance
    if (!isLoggedIn) {
        cout << "Error: Please log in first\n";
        return;
    }

    cout << "\nAccount Balance\n";
    cout << "---------------\n";
    cout << "Username: " << currentUser.id_name << "\n";
    cout << "Current balance: $" << currentUser.balance << "\n";
}

void logout() {
    // Function to end user session
    // 1. Resets login status
    // 2. Clears current user information
    isLoggedIn = false;
    currentUser = Account(); // Reset current user info
    cout << "Logged out successfully!\n";
}

bool withdraw_amount() {
    // Function to withdraw money
    // 1. Requires user to be logged in
    // 2. Checks for sufficient balance
    // 3. Updates balance in both file and current session
    if (!isLoggedIn) {
        cout << "Error: Please log in first\n";
        return false;
    }

    double amount;
    cout << "Withdraw Money\n";
    cout << "--------------\n";
    cout << "Current balance: $" << currentUser.balance << "\n";
    cout << "Enter amount to withdraw: $";
    
    while (!(cin >> amount) || amount <= 0) {
        cout << "Please enter a valid positive amount: $";
        clearInputBuffer();
    }

    // Check if sufficient balance
    if (amount > currentUser.balance) {
        cout << "Error: Insufficient balance\n";
        return false;
    }

    // Read account file
    ifstream readFile("account.txt");
    ofstream tempFile("account_temp.txt");
    
    if (!readFile || !tempFile) {
        cerr << "Error: Unable to process files\n";
        return false;
    }

    string line;
    bool found = false;
    
    while (getline(readFile, line)) {
        size_t pos = 0;
        string delimiter = "|";
        vector<string> tokens;
        
        while ((pos = line.find(delimiter)) != string::npos) {
            tokens.push_back(line.substr(0, pos));
            line.erase(0, pos + delimiter.length());
        }
        tokens.push_back(line);

        if (tokens[1] == currentUser.id) {
            found = true;
            double currentBalance = stod(tokens[4]);
            double newBalance = currentBalance - amount;
            
            // Update both file and current user's balance
            tempFile << tokens[0] << "|" << tokens[1] << "|" 
                    << tokens[2] << "|" << tokens[3] << "|" << newBalance << "\n";
            currentUser.balance = newBalance;
                    
            cout << "\nWithdrawal successful!\n";
            cout << "Previous balance: $" << currentBalance << "\n";
            cout << "Withdrawn amount: $" << amount << "\n";
            cout << "New balance: $" << newBalance << "\n";
        } else {
            tempFile << line << "\n";
        }
    }

    readFile.close();
    tempFile.close();

    if (found) {
        remove("account.txt");
        rename("account_temp.txt", "account.txt");
        return true;
    }

    remove("account_temp.txt");
    cout << "Error: Account not found\n";
    return false;
}

// Main menu function
int main() {
    // Main program loop
    // 1. Shows main menu for non-logged-in users:
    //    - Create account
    //    - Generate new PIN
    //    - Login
    //    - Forgot PIN
    //    - Exit
    // 2. Shows account menu for logged-in users:
    //    - Deposit
    //    - Check balance
    //    - Withdraw
    //    - Logout
    srand(time(0));

    while (true) {
        cout << "\nATM Management System\n";
        cout << "--------------------\n";
        cout << "1. Create new account\n";
        cout << "2. Generate new PIN\n";
        cout << "3. Login to account\n";
        cout << "4. Forgot PIN\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                make_account();
                break;
            case 2:
                generate_pin();
                break;
            case 3:
                if (log_in()) {
                    while (true) {
                        cout << "\nAccount Menu\n";
                        cout << "------------\n";
                        cout << "1. Deposit money\n";
                        cout << "2. Check balance\n";
                        cout << "3. Withdraw money\n";
                        cout << "4. Logout\n";
                        cout << "Enter your choice: ";
                        
                        int subchoice;
                        cin >> subchoice;
                        
                        switch (subchoice) {
                            case 1:
                                store_amount();
                                break;
                            case 2:
                                check_amount();
                                break;
                            case 3:
                                withdraw_amount();
                                break;
                            case 4:
                                logout();
                                goto main_menu;
                            default:
                                cout << "Invalid choice\n";
                        }
                    }
                }
                main_menu:
                break;
            case 4:
                forget_pin();
                break;
            case 5:
                cout << "Thank you for using ATM Management System!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}