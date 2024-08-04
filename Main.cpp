#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib> // for system("cls")
#include <conio.h> // for _getch()
#include <regex>
#include <ctime>
#include <windows.h>

using namespace std;

const int SuccessMessagesColorCode = 10;
const int WaitingMessagesColorCode = 11;
const int ErrorMessagesColorCode = 12;
const int TittleColorCode = 6;
const int PressKeyColorCode = 8;
const int DefaultColorCode = 9;

void setConsoleForegroundColor(int colorCode)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
}
void PrintColoredText(const string &text, int color)
{
    setConsoleForegroundColor(color);
    cout << "\t\t\t\t\t\t\t" << text << endl;
    setConsoleForegroundColor(DefaultColorCode);
}
void PrintColoredTittle(const string &text, int color)
{
    setConsoleForegroundColor(color);
    cout << "\t\t\t\t" << text << endl;
    setConsoleForegroundColor(DefaultColorCode);
}
void PrintErrorsORSucess(const string &text, int color)
{
    setConsoleForegroundColor(color);
    cout << "\t\t\t\t\t\t\t\t" << text << endl;
    setConsoleForegroundColor(DefaultColorCode);
}
void ShowMenuOptions(int num, string &option)
{
    setConsoleForegroundColor(TittleColorCode);
    cout << "\t\t\t\t\t\t\t" << num << ". ";
    setConsoleForegroundColor(DefaultColorCode);
    cout << option << endl;
}
void InputTaking(const string &text)
{
    setConsoleForegroundColor(TittleColorCode);
    cout << "\t\t\t\t\t\t\t" << text << ": ";
    setConsoleForegroundColor(DefaultColorCode);
}
// <==================== Checking and Installing Dependencies   =================>
bool checkDependency(const string &command, const string &name)
{
    PrintColoredText("Checking if " + name + " is installed or not...", WaitingMessagesColorCode);
    bool isInstalled = system(command.c_str()) == 0;
    if (isInstalled)
    {
        PrintColoredText(name + " is installed.", SuccessMessagesColorCode);
    }
    else
    {
        PrintColoredText(name + " is not installed.", ErrorMessagesColorCode);
    }
    return isInstalled;
}

void installDependency(const string &installCommand, const string &name)
{
    PrintColoredText("Installing " + name + "...", WaitingMessagesColorCode);
    system(installCommand.c_str());
}
bool checkAndInstallDependencies()
{
    bool npmInstalled = checkDependency("npm -v > nul 2>&1", "npm");
    bool nodeInstalled = checkDependency("node -v > nul 2>&1", "Node.js");
    bool appwriteInstalled = checkDependency("npm list appwrite > nul 2>&1", "Appwrite");
    bool argon2Installed = checkDependency("npm list argon2 > nul 2>&1", "Argon2");

    if (!nodeInstalled && !npmInstalled)
    {
        PrintErrorsORSucess("Node.js or npm is required to run this program. Please download and install Node.js from https://nodejs.org/en/.", ErrorMessagesColorCode);
        return false;
    }

    if (!appwriteInstalled || !argon2Installed)
    {
        char choice;

        InputTaking("Some packages are missing. Would you like to install them? (y/n)");
        cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            if (!appwriteInstalled)
            {
                installDependency("npm install -g appwrite", "Appwrite");
            }
            if (!argon2Installed)
            {
                installDependency("npm install argon2", "Argon2");
            }
            return true;
        }
        else
        {
            if (!appwriteInstalled)
            {
                PrintColoredText("Appwrite is required to run this program. Exiting...", ErrorMessagesColorCode);
            }
            if (!argon2Installed)
            {
                PrintColoredText("Argon2 is required to run this program. Exiting...", ErrorMessagesColorCode);
            }
            return false;
        }
    }

    PrintColoredText("All dependencies are installed.", SuccessMessagesColorCode);
    return true;
}
string callJavaScript(const string &command)
{
    string cmd = "node Server.js " + command + " > output.txt";
    system(cmd.c_str());

    ifstream file("output.txt");
    string result;
    getline(file, result);
    file.close();

    ofstream clearFile("output.txt", ios::trunc);
    clearFile.close();
    return result;
}
// FetchAllUsersFromJavaScript
string FetchAllLargeDataFromJavaScript(const string &command)
{
    string cmd = "node Server.js " + command + " > output.txt";
    system(cmd.c_str());

    ifstream file("output.txt");
    stringstream buffer;
    buffer << file.rdbuf();
    string result = buffer.str();
    file.close();

    ofstream clearFile("output.txt", ios::trunc);
    clearFile.close();

    return result;
}

// Base class to represent a bank account
class Account
{
protected:
    // Struct to represent a transaction
    struct Transaction
    {
        string timestamp;
        string username;
        string type; // "Deposit", "Withdrawal" or "Transfer"
        double amount;
    };
    string cnic;
    string username;
    string password;
    string email;
    double balance;
    vector<Transaction> transactions; // Transaction history

public:
    string getCurrentTimestamp()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        stringstream ss;
        ss << 1900 + ltm->tm_year << "-"
           << 1 + ltm->tm_mon << "-"
           << ltm->tm_mday << "_"
           << ltm->tm_hour << ":"
           << ltm->tm_min << ":"
           << ltm->tm_sec;
        return ss.str();
    }

    Account(string _cnic, string _username, string _password, string _email, double _balance = 0)
        : cnic(_cnic), username(_username), password(_password), email(_email), balance(_balance) {}

    virtual ~Account() {}

    string getUsername() const { return username; }

    string getCNIC() const { return cnic; }

    string getEmail() const { return email; }

    double getBalance() const { return balance; }

    void logTransaction(const string &cnic, const string &timestamp, const string &type, double amount, const string &recipientCnic = "")
    {
        string command = "LogTransaction " + cnic + " " + timestamp + " " + type + " " + to_string(amount);
        if (!recipientCnic.empty())
        {
            command += " " + recipientCnic;
        }
        string result = callJavaScript(command);
    }
    void UpdateBalanceInBackend()
    {
        string command = "BalanceUpdate " + cnic + " " + to_string(balance);
        string result = callJavaScript(command);
    }

    void deposit(double amount, string type = "")
    {
        balance += amount;
        string timestamp = getCurrentTimestamp();
        transactions.push_back({timestamp, username, type, amount});
        if (type != "Received" && type != "Transfer")
        {
            logTransaction(cnic, timestamp, type, amount); // Log transaction
        }
        UpdateBalanceInBackend();
    }

    bool withdraw(double amount, string type)
    {
        if (balance >= amount)
        {
            balance -= amount;
            string timestamp = getCurrentTimestamp();
            transactions.push_back({timestamp, username, type, amount});
            if (type != "Received" && type != "Transfer")
            {
                logTransaction(cnic, timestamp, type, amount); // Log transaction
            }
            UpdateBalanceInBackend();
            return true;
        }
        return false;
    }

    void changeUsername(string new_username)
    {
        username = new_username;
    }

    // Function to display user menu
    virtual void displayMenu()
    {
        PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
        PrintColoredTittle("|                                                                                     |", TittleColorCode);
        PrintColoredTittle("|                                  User Dashoard                                      |", TittleColorCode);
        PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);

        cout << "\t\t\t\t\t\t\tWelcome, " << getUsername() << endl;
        cout << "\t\t\t\t\t\t\tYour balance is: $" << getBalance() << endl;
        string options[] = {"Deposit",
                            "Withdraw",
                            "Change Password",
                            "Change Username",
                            "View Transaction History",
                            "Transfer Money",
                            "Logout"};
        int size = sizeof(options) / sizeof(options[0]);
        for (int i = 0; i < size; i++)
        {
            ShowMenuOptions(i + 1, options[i]);
        }
    }

    void viewTransactionHistory(string cnic) const
    {
        PrintErrorsORSucess("Hold on getting your transactions...", WaitingMessagesColorCode);
        string command = "FetchTransactions " + cnic;
        string result = FetchAllLargeDataFromJavaScript(command);
        system("cls");
        cout << "\n\n";
        PrintColoredText("Transaction History for User " + getUsername() + " :", TittleColorCode);

        if (result.empty())
        {
            PrintErrorsORSucess("No transactions found.\n", ErrorMessagesColorCode);
        }
        else
        {
            cout << result << endl;
        }

        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }

    virtual string getAccountType() const = 0;
};

// Derived class for Savings Account
class SavingsAccount : public Account
{
public:
    SavingsAccount(string _cnic, string _username, string _password, string _email, double _balance = 0)
        : Account(_cnic, _username, _password, _email, _balance) {}

    string getAccountType() const override
    {
        return "Savings";
    }
};

// Derived class for Current Account
class CurrentAccount : public Account
{
public:
    CurrentAccount(string _cnic, string _username, string _password, string _email, double _balance = 0)
        : Account(_cnic, _username, _password, _email, _balance) {}

    string getAccountType() const override
    {
        return "Current";
    }
};

// Class to manage accounts
class Bank
{
private:
    vector<Account *> accounts;
    vector<Account *> pendingAccounts;

public:
    Bank()
    {
        loadAccountsFromBackend();
    }

    ~Bank()
    {
        for (auto account : accounts)
        {
            delete account;
        }
        for (auto account : pendingAccounts)
        {
            delete account;
        }
    }
    void LoadAllAccountsFromBackend()
    {
        accounts.clear();
        pendingAccounts.clear();
        loadAccountsFromBackend();
        loadPendingAccountsFromBackend();
    }
    void loadAccountsFromBackend()
    {
        string command = "FetchAccounts"; // Command to fetch all user details
        string result = FetchAllLargeDataFromJavaScript(command);

        if (result.empty())
        {
            PrintErrorsORSucess("Failed to Connect to Database. Please try again.", ErrorMessagesColorCode);
            PrintColoredText("Press any key to exit...", PressKeyColorCode);
            _getch(); // Wait for user input
            exit(0);  // Exit the program
        }
        // Split the result string into lines
        stringstream ss(result);
        string line;

        while (getline(ss, line))
        {
            if (line.empty())
                continue; // Skip empty lines

            // Split each line into account details
            stringstream lineStream(line);
            string username, email, accountType, cnic;
            double balance;

            lineStream >> username >> email >> accountType >> balance >> cnic;

            // Create account objects based on the account type
            Account *account = nullptr;
            if (accountType == "Savings")
            {
                account = new SavingsAccount(cnic, username, "", email, balance);
            }
            else
            {
                account = new CurrentAccount(cnic, username, "", email, balance);
            }

            // Add the account object to the accounts vector
            accounts.push_back(account);
        }

        // Displaying the fetched result for verification
        // cout << "Press any key to continue...";
        // _getch();
    }

    void loadPendingAccountsFromBackend()
    {
        string command = "FetchPendingAccounts"; // Command to fetch all user details
        string result = FetchAllLargeDataFromJavaScript(command);

        // Split the result string into lines
        stringstream ss(result);
        string line;

        while (getline(ss, line))
        {
            if (line.empty())
                continue; // Skip empty lines

            // Split each line into account details
            stringstream lineStream(line);
            string username, email, accountType, cnic;
            double balance;

            lineStream >> username >> email >> accountType >> cnic;

            // Create account objects based on the account type
            Account *pendingaccount = nullptr;
            if (accountType == "Savings")
            {
                pendingaccount = new SavingsAccount(cnic, username, "", email, 0);
                ;
            }
            else
            {
                pendingaccount = new CurrentAccount(cnic, username, "", email, 0);
            }

            // Add the account object to the accounts vector
            pendingAccounts.push_back(pendingaccount);
        }
    }
    bool authenticateAdmin(string username, string password)
    {
        string command = "AdminLogin " + username + " " + password;
        string result = callJavaScript(command);
        return (result == "SUCCESS" ? true : false);
    }

    Account *authenticateUser(string cnic, string password)
    {
        PrintErrorsORSucess("Proccessing your request...", WaitingMessagesColorCode);
        string command = "UserLogin " + cnic + " " + password;
        string result = callJavaScript(command);
        if (result == "SUCCESS")
        {
            PrintErrorsORSucess("Logging in...", WaitingMessagesColorCode);
            accounts.clear();
            loadAccountsFromBackend();
            PrintErrorsORSucess("Getting your dashboard ready...", WaitingMessagesColorCode);
            // Search for the user in the loaded accounts vector
            for (auto account : accounts)
            {
                if (account->getCNIC() == cnic)
                {
                    // If CNIC match, return the account object
                    return account;
                }
            }
        }

        // If user not found , return nullptr
        return nullptr;
    }
    void createAccount(Account *account, bool isPending = false)
    {
        if (isPending)
        {
            pendingAccounts.push_back(account);
        }
        else
        {
            accounts.push_back(account);
        }
    }

    void approvePendingAccounts()
    {
        // Doing this so that if user creates account from anywhere then Admin should see the account immediately
        pendingAccounts.clear();
        loadPendingAccountsFromBackend();
        if (pendingAccounts.empty())
        {
            PrintErrorsORSucess("No pending accounts to approve.\n", ErrorMessagesColorCode);
        }
        else
        {
            cout << "\n\n";
            PrintColoredText("Pending Accounts:\n", TittleColorCode);
            cout << "\t\t\t\t\t\t\tNo.\tCNIC\t\t\tUsername\n";
            cout << "\t\t\t\t\t\t\t-----------------------------------------\n";

            // Iterate over pendingAccounts and print details in table format
            for (size_t i = 0; i < pendingAccounts.size(); ++i)
            {
                cout << "\t\t\t\t\t\t\t" << i + 1 << ".\t"
                     << pendingAccounts[i]->getCNIC() << "\t\t"
                     << pendingAccounts[i]->getUsername() << "\n";
                cout << "\t\t\t\t\t\t\t-----------------------------------------\n";
            }
            cout << "\t\t\t\t\t\t\t-----------------------------------------\n";

            int choice;
            InputTaking("Enter the number of the account to approve (0 to exit)");
            cin >> choice;

            if (choice > 0 && choice <= pendingAccounts.size())
            {
                string cnic = pendingAccounts[choice - 1]->getCNIC();

                string command = "MoveToAccounts " + cnic;
                string result = callJavaScript(command);

                if (result == "ACCOUNT MOVED TO ACCOUNTS SUCCESSFULLY")
                {
                    // Move the account from pendingAccounts to accounts
                    accounts.push_back(pendingAccounts[choice - 1]);
                    pendingAccounts.erase(pendingAccounts.begin() + choice - 1);
                    PrintErrorsORSucess("Account approved and moved to main accounts.", SuccessMessagesColorCode);
                }
                else
                {
                    PrintErrorsORSucess("Failed to move user to main accounts.", ErrorMessagesColorCode);
                }
            }
        }
        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }

    void changeUserPassword(string cnic, string new_password)
    {
        PrintErrorsORSucess("Hold on changing your Password...", WaitingMessagesColorCode);
        string command = "ChangeUserPassword " + cnic + " " + new_password;
        string result = callJavaScript(command);

        PrintErrorsORSucess("Password Changed Successfully\n", SuccessMessagesColorCode);
        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }
    void changeUserUsername(string cnic, string new_username, Account *account)
    {
        PrintErrorsORSucess("Hold on changing your Username...", WaitingMessagesColorCode);
        string command = "ChangeUserUsername " + cnic + " " + new_username;
        string result = callJavaScript(command);

        if (result == "USERNAME_UPDATED")
        {
            account->changeUsername(new_username);
            PrintErrorsORSucess("Username changed successfully.", SuccessMessagesColorCode);
        }
        else
        {
            PrintErrorsORSucess("Username change failed.", ErrorMessagesColorCode);
        }
        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }

    void viewAllAccounts()
    {
        accounts.clear();
        loadAccountsFromBackend();
        if (accounts.empty())
        {
            PrintErrorsORSucess("No accounts found.\n", ErrorMessagesColorCode);
        }
        else
        {
            cout << "\n\n";
            PrintColoredText("All Accounts:\n", TittleColorCode);
            for (const auto &account : accounts)
            {
                cout << "\t\t\t\t\t\t\tCNIC: " << account->getCNIC() << ", Username: " << account->getUsername()
                     << ", Email: " << account->getEmail() << ", Balance: $" << account->getBalance()
                     << ", Type: " << account->getAccountType() << endl;
            }
        }
        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }

    void removeAccount()
    {
        accounts.clear();
        loadAccountsFromBackend();
        if (accounts.empty())
        {
            PrintErrorsORSucess("No accounts to remove.\n", ErrorMessagesColorCode);
        }
        else
        {
            cout << "\n\n";
            PrintColoredText("Accounts:\n", TittleColorCode);
            for (size_t i = 0; i < accounts.size(); ++i)
            {
                cout << "\t\t\t\t\t\t\t" << i + 1 << ". CNIC: " << accounts[i]->getCNIC() << ", Username: " << accounts[i]->getUsername()
                     << ", Email: " << accounts[i]->getEmail() << ", Balance: $" << accounts[i]->getBalance() << endl;
            }

            int choice;
            InputTaking("Enter the number of the account to remove (0 to exit)");
            cin >> choice;

            if (choice > 0 && choice <= accounts.size())
            {
                string command = "DeleteAccount " + accounts[choice - 1]->getCNIC();
                string result = callJavaScript(command);
                if (result == "ACCOUNT_DELETED")
                {
                    delete accounts[choice - 1];
                    accounts.erase(accounts.begin() + choice - 1);
                    PrintErrorsORSucess("Account removed.\n", SuccessMessagesColorCode);
                }
                else
                {
                    PrintErrorsORSucess("Failed to delete account make sure you have an active internet connection.", ErrorMessagesColorCode);
                }
            }
        }
        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }

    bool isNumeric(const string &str)
    {
        for (char const &c : str)
        {
            if (isdigit(c) == 0)
                return false;
        }
        return true;
    }

    double getValidatedAmount()
    {
        double amount;
        string input;
        while (true)
        {
            InputTaking("Enter amount");
            cin >> input;
            if (isNumeric(input))
            {
                stringstream ss(input);
                ss >> amount;
                if (amount > 0)
                {
                    break;
                }
                else
                {
                    PrintErrorsORSucess("Amount should be positive.\n", ErrorMessagesColorCode);
                }
            }
            else
            {
                PrintErrorsORSucess("Invalid amount. Please enter a numeric value.\n", ErrorMessagesColorCode);
            }
        }
        return amount;
    }

    string getPasswordInput()
    {
        string password;
        char ch;
        while ((ch = _getch()) != '\r')
        { // '\r' is the Enter key
            if (ch == '\b')
            { // '\b' is the Backspace key
                if (!password.empty())
                {
                    password.pop_back();
                    cout << "\b \b"; // Erase character from console
                }
            }
            else
            {
                password += ch;
                cout << '*'; // Print asterisk instead of character
            }
        }
        cout << endl;
        return password;
    }

    void transferMoney(Account *from_account)
    {
        LoadAllAccountsFromBackend();
        string recipient_CNIC;
        InputTaking("Enter recipient's CNIC");
        cin >> recipient_CNIC;

        // Check if the recipient CNIC is the same as the sender's CNIC
        if (from_account->getCNIC() == recipient_CNIC)
        {
            PrintErrorsORSucess("You cannot transfer money to the your account.\n", ErrorMessagesColorCode);
            PrintColoredText("Press any key to continue...", PressKeyColorCode);
            _getch(); // Wait for user input
            return;
        }

        Account *recipient_account = nullptr;
        for (auto account : accounts)
        {
            if (account->getCNIC() == recipient_CNIC)
            {
                recipient_account = account;
                break;
            }
        }

        if (recipient_account)
        {
            double amount = getValidatedAmount();
            PrintErrorsORSucess("Initiating transfer process...", WaitingMessagesColorCode);

            // cout << "Attempting to withdraw amount: " << amount << " from " << from_account->getUsername() << endl;

            if (from_account->withdraw(amount, "Transfer"))
            {
                PrintErrorsORSucess("Amount deducted successfully from " + from_account->getUsername() + "'s account.", SuccessMessagesColorCode);
                stringstream stream;
                stream << std::fixed;
                stream.precision(2);
                stream << amount;
                string amountStr = stream.str();
                PrintErrorsORSucess("Transferring $" + amountStr + " to " + recipient_account->getUsername() + "...", WaitingMessagesColorCode);
                recipient_account->deposit(amount, "Received");

                string timestamp = from_account->getCurrentTimestamp();
                // cout << "Logging transfer transaction for sender.\n";
                from_account->logTransaction(from_account->getCNIC(), timestamp, "Transfer", amount, recipient_CNIC);

                // cout << "Logging transfer transaction for recipient.\n";
                recipient_account->logTransaction(recipient_account->getCNIC(), timestamp, "Received", amount, from_account->getCNIC());

                PrintErrorsORSucess("Transfer completed successfully!", SuccessMessagesColorCode);
                PrintErrorsORSucess("The amount has been transferred.", SuccessMessagesColorCode);
            }

            else
            {
                PrintErrorsORSucess("Insufficient balance for transfer.\n", ErrorMessagesColorCode);
            }
        }
        else
        {
            PrintErrorsORSucess("Recipient account not found.\n", ErrorMessagesColorCode);
        }
        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }

    void start()
    {
        while (true)
        {
            system("cls");
            cout << "\n\n";
            PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
            PrintColoredTittle("|                                                                                     |", TittleColorCode);
            PrintColoredTittle("|                           Welcome to Bank Management System                         |", TittleColorCode);
            PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);

            string options[] = {"Login",
                                "Create Account",
                                "Admin Login",
                                "Exit"};
            int size = sizeof(options) / sizeof(options[0]);
            for (int i = 0; i < size; i++)
            {
                ShowMenuOptions(i + 1, options[i]);
            }
            int choice;
            InputTaking("Enter your choice");
            cin >> choice;

            switch (choice)
            {
            case 1:
                login();
                break;
            case 2:
                createAccount();
                break;
            case 3:
                adminLogin();
                break;
            case 4:
                return;
            default:
                PrintErrorsORSucess("Invalid choice. Please try again.\n", ErrorMessagesColorCode);
                _getch(); // Wait for user input
                break;
            }
        }
    }

    void login()
    {
        system("cls");
        cout << "\n\n";
        PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
        PrintColoredTittle("|                                                                                     |", TittleColorCode);
        PrintColoredTittle("|                                      User Login                                     |", TittleColorCode);
        PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);

        string cnic, password;
        InputTaking("Enter CNIC");
        cin >> cnic;
        InputTaking("Enter password");
        password = getPasswordInput();

        Account *account = authenticateUser(cnic, password);
        if (account)
        {
            userMenu(account);
        }
        else
        {
            PrintErrorsORSucess("Invalid CNIC or password.", ErrorMessagesColorCode);
            PrintColoredText("Press any key to continue...", PressKeyColorCode);
            _getch(); // Wait for user input
        }
    }

    bool CheckCNIC(string cnic)
    {
        // Assume CNIC is not in use initially
        bool cnicAvailable = true;
        PrintErrorsORSucess("Please wait while your CNIC is being verified...", WaitingMessagesColorCode);
        LoadAllAccountsFromBackend();
        // Check if CNIC already exists in loaded accounts
        for (const auto &account : accounts)
        {
            if (account->getCNIC() == cnic)
            {
                cnicAvailable = false;
                break;
            }
        }

        // Check if CNIC already exists in pending accounts
        if (cnicAvailable)
        {
            for (const auto &account : pendingAccounts)
            {
                if (account->getCNIC() == cnic)
                {
                    cnicAvailable = false;
                    break;
                }
            }
        }
        return cnicAvailable; // Return whether CNIC is available or not
    }
    bool CheckEmail(string email)
    {
        // Assume email is not in use initially
        bool emailAvailable = true;
        PrintErrorsORSucess("Please wait while your EMAIL is being verified...", WaitingMessagesColorCode);
        LoadAllAccountsFromBackend();
        // Check if email already exists in loaded accounts
        for (const auto &account : accounts)
        {
            if (account->getEmail() == email)
            {
                emailAvailable = false;
                break;
            }
        }

        // Check if email already exists in pending accounts
        if (emailAvailable)
        {
            for (const auto &account : pendingAccounts)
            {
                if (account->getEmail() == email)
                {
                    emailAvailable = false;
                    break;
                }
            }
        }
        return emailAvailable; // Return whether email is available or not
    }

    void createAccount()
    {
        system("cls");
        cout << "\n\n";
        PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
        PrintColoredTittle("|                                                                                     |", TittleColorCode);
        PrintColoredTittle("|                                      User Registration                              |", TittleColorCode);
        PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);
        string command;
        string cnic, username, password, email, account_type;
        double initial_deposit = 0;

        while (true)
        {
            InputTaking("Enter CNIC (xxxxx-xxxxxxx-x)");
            cin >> cnic;

            if (!regex_match(cnic, regex(R"(^\d{5}-\d{7}-\d{1}$)")))
            {
                PrintErrorsORSucess("Invalid CNIC. Please enter a valid 13-digit CNIC including dashes (\"-\").", ErrorMessagesColorCode);
            }
            else if (!CheckCNIC(cnic))
            {
                PrintErrorsORSucess("CNIC is already in use. Please enter a different CNIC.", ErrorMessagesColorCode);
            }
            else
            {
                break; // Email is unique, exit the loop
            }
        }

        while (true)
        {
            InputTaking("Enter username");
            cin >> username;
            if (username.length() < 3)
            {
                PrintErrorsORSucess("Username must be at least 3 characters", ErrorMessagesColorCode);
            }
            else
            {
                break;
            }
        }

        while (true)
        {
            InputTaking("Enter password");
            cin >> password;
            if (password.length() < 8)
            {
                PrintErrorsORSucess("Password must be at least 8 characters", ErrorMessagesColorCode);
            }
            else
            {
                break;
            }
        }

        while (true)
        {
            InputTaking("Enter email");
            cin >> email;

            if (!regex_match(email, regex(".+@.+\\..+")))
            {
                PrintErrorsORSucess("Invalid email format. Please enter a valid email.", ErrorMessagesColorCode);
            }
            else if (!CheckEmail(email))
            {
                PrintErrorsORSucess("Email is already in use. Please enter a different email.", ErrorMessagesColorCode);
            }
            else
            {
                break; // Email is unique, exit the loop
            }
        }
        while (true)
        {
            InputTaking("Enter account type (Savings 'S' /Current 'C')");
            cin >> account_type;
            if (account_type == "S" || account_type == "s")
            {
                createAccount(new SavingsAccount(cnic, username, password, email, initial_deposit), true);
                break;
            }
            else if (account_type == "C" || account_type == "c")
            {
                createAccount(new CurrentAccount(cnic, username, password, email, initial_deposit), true);
                break;
            }
            else
            {
                PrintErrorsORSucess("Invalid account type. Please try again.", ErrorMessagesColorCode);
            }
        }
        PrintErrorsORSucess("Hold on creating your account.", WaitingMessagesColorCode);

        command = "Register " + username + " " + password + " " + email + " " + account_type + " " + cnic;
        callJavaScript(command);
        PrintErrorsORSucess("Account created successfully. Pending admin approval.", SuccessMessagesColorCode);

        PrintColoredText("Press any key to continue...", PressKeyColorCode);
        _getch(); // Wait for user input
    }

    void adminLogin()
    {
        system("cls");
        cout << "\n\n";
        PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
        PrintColoredTittle("|                                                                                     |", TittleColorCode);
        PrintColoredTittle("|                                      Admin Login                                    |", TittleColorCode);
        PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);
        string username, password;
        InputTaking("Enter admin username");
        cin >> username;
        InputTaking("Enter admin password");
        password = getPasswordInput();

        if (authenticateAdmin(username, password))
        {
            adminMenu();
        }
        else
        {
            PrintErrorsORSucess("Invalid admin username or password.\n", ErrorMessagesColorCode);
            PrintColoredText("Press any key to continue...", PressKeyColorCode);
            _getch(); // Wait for user input
        }
    }

    void adminMenu()
    {
        while (true)
        {
            system("cls");
            cout << "\n\n";
            PrintColoredTittle(" _____________________________________________________________________________________", TittleColorCode);
            PrintColoredTittle("|                                                                                     |", TittleColorCode);
            PrintColoredTittle("|                                  Admin Dashoard                                     |", TittleColorCode);
            PrintColoredTittle("|_____________________________________________________________________________________|\n", TittleColorCode);

            string options[] = {"Approve Pending Accounts",
                                "View All Accounts",
                                "Remove Accounts",
                                "Logout"};
            int size = sizeof(options) / sizeof(options[0]);
            for (int i = 0; i < size; i++)
            {
                ShowMenuOptions(i + 1, options[i]);
            }
            int choice;
            InputTaking("Enter your choice");
            cin >> choice;

            switch (choice)
            {
            case 1:
                system("cls");
                approvePendingAccounts();
                break;
            case 2:
                system("cls");
                viewAllAccounts();
                break;
            case 3:
                system("cls");
                removeAccount();
                break;
            case 4:
                return;
            default:
                PrintErrorsORSucess("Invalid choice. Please try again.\n", ErrorMessagesColorCode);
                _getch(); // Wait for user input
                break;
            }
        }
    }

    void userMenu(Account *account)
    {
        while (true)
        {
            system("cls");
            cout << "\n\n";
            account->displayMenu();
            int choice;
            InputTaking("Enter your choice");
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                double amount = getValidatedAmount();
                PrintErrorsORSucess("Initiating Deposit process...", WaitingMessagesColorCode);
                account->deposit(amount, "Deposit");
                PrintErrorsORSucess("Deposit successful.\n", SuccessMessagesColorCode);
                PrintColoredText("Press any key to continue...", PressKeyColorCode);
                _getch(); // Wait for user input
                break;
            }
            case 2:
            {
                double amount = getValidatedAmount();
                PrintErrorsORSucess("Initiating Withdraw process...", WaitingMessagesColorCode);
                if (account->withdraw(amount, "Withdrawal"))
                {
                    PrintErrorsORSucess("Withdrawal successful.\n", SuccessMessagesColorCode);
                }
                else
                {
                    PrintErrorsORSucess("Insufficient balance.\n", ErrorMessagesColorCode);
                }
                PrintColoredText("Press any key to continue...", PressKeyColorCode);
                _getch(); // Wait for user input
                break;
            }
            case 3:
            {
                while (true)
                {
                    string new_password;
                    InputTaking("Enter new password");
                    cin >> new_password;

                    if (new_password.length() < 8)
                    {
                        PrintErrorsORSucess("Password must be at least 8 characters", ErrorMessagesColorCode);
                    }
                    else
                    {
                        changeUserPassword(account->getCNIC(), new_password);
                        break;
                    }
                }
                break;
            }
            case 4:
            {
                while (true)
                {
                    string new_username;
                    InputTaking("Enter new username");
                    cin >> new_username;

                    if (new_username.length() < 3)
                    {
                        PrintErrorsORSucess("Username must be at least 3 characters", ErrorMessagesColorCode);
                    }
                    else
                    {
                        changeUserUsername(account->getCNIC(), new_username, account);
                        break;
                    }
                }
                break;
            }
            case 5:
                system("cls");
                account->viewTransactionHistory(account->getCNIC());
                break;
            case 6:
                transferMoney(account);
                break;
            case 7:
                return;
            default:
                PrintErrorsORSucess("Invalid choice. Please try again.\n", ErrorMessagesColorCode);
                _getch(); // Wait for user input
                break;
            }
        }
    }
};

int main()
{
    if (!checkAndInstallDependencies())
    {
        return 1; // Exit if dependencies are not installed
    }

    setConsoleForegroundColor(DefaultColorCode);
    // Purple=5 Yellow=6 gray=8 lightBlue=9
    Bank bank;

    bank.start();

    return 0;
}
