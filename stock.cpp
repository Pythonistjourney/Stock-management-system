#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <conio.h> // For _getch() function on Windows

using namespace std;


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



// Class for Stock

class Stock {
public:
    string name;
    int quantity;
    double price;
    bool approved;

    Stock(const string &name, int quantity, double price, bool approved)
        : name(name), quantity(quantity), price(price), approved(approved) {}
};

// Base class for User
class User {
protected:
    string username;
    string password;

public:
    User(const string &username, const string &password)
        : username(username), password(password) {}

    virtual ~User() = default;
    virtual void displayMenu() const = 0;
    virtual void handleInput(int choice) = 0;
};

// Dealer class inheriting from User
class Dealer : public User {
private:
    vector<unique_ptr<Stock>> stockList;

public:
    Dealer(const string &username, const string &password)
        : User(username, password) {}

    void displayMenu() const override {
        cout << "1. Insert Stock\n";
        cout << "2. Update Stock\n";
        cout << "3. Delete Stock\n";
        cout << "4. View Stock\n";
        cout << "5. Approve Stock\n";
        cout << "6. Logout\n";
    }

    void handleInput(int choice) override {
        switch (choice) {
            case 1:
                insertStock();
                break;
            case 2:
                updateStock();
                break;
            case 3:
                deleteStock();
                break;
            case 4:
                viewStock();
                break;
            case 5:
                approveStock();
                break;
            case 6:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }

    void insertStock() {
        string name;
        int quantity;
        double price;

        cout << "Enter stock name: ";
        cin >> name;
        cout << "Enter stock quantity: ";
        cin >> quantity;
        cout << "Enter stock price: ";
        cin >> price;

        stockList.push_back(make_unique<Stock>(name, quantity, price, false));
        cout << "Stock inserted successfully.\n";
    }

    void updateStock() {
        string name;
        cout << "Enter stock name to update: ";
        cin >> name;

        for (auto &stock : stockList) {
            if (stock->name == name) {
                int quantity;
                double price;
                cout << "Enter new quantity: ";
                cin >> quantity;
                cout << "Enter new price: ";
                cin >> price;

                stock->quantity = quantity;
                stock->price = price;
                cout << "Stock updated successfully.\n";
                return;
            }
        }
        cout << "Stock not found.\n";
    }

    void deleteStock() {
        string name;
        cout << "Enter stock name to delete: ";
        cin >> name;

        for (auto it = stockList.begin(); it != stockList.end(); ++it) {
            if ((*it)->name == name) {
                stockList.erase(it);
                cout << "Stock deleted successfully.\n";
                return;
            }
        }
        cout << "Stock not found.\n";
    }

    void viewStock() const {
        for (const auto &stock : stockList) {
            cout << "Name: " << stock->name
                 << ", Quantity: " << stock->quantity
                 << ", Price: " << stock->price
                 << ", Approved: " << (stock->approved ? "Yes" : "No") << endl;
        }
    }

    void approveStock() {
        string name;
        cout << "Enter stock name to approve: ";
        cin >> name;

        for (auto &stock : stockList) {
            if (stock->name == name) {
                stock->approved = true;
                cout << "Stock approved successfully.\n";
                return;
            }
        }
        cout << "Stock not found.\n";
    }

    const vector<unique_ptr<Stock>>& getStockList() const {
        return stockList;
    }
};

// Customer class inheriting from User
class Customer : public User {
private:
    vector<unique_ptr<Stock>> selectedStocks;

public:
    Customer(const string &username, const string &password)
        : User(username, password) {}

    void displayMenu() const override {
        cout << "1. View Stock\n";
        cout << "2. Buy Stock\n";
        cout << "3. Logout\n";
    }

    void handleInput(int choice) override {
        switch (choice) {
            case 1:
                viewStock();
                break;
            case 2:
                buyStock();
                break;
            case 3:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }

    void viewStock() const {
        // Assuming you have access to the stockList from Dealer
        // Here, we're simulating by calling a placeholder function
        fetchStockFromServer();
    }

    void buyStock()  {
    vector<Stock> availableStocks = fetchStockFromServer(); // Fetch stocks
    int choice = -1;
    while (choice != 0) {
        cout << "Available Stocks:\n";
        for (size_t i = 0; i < availableStocks.size(); ++i) {
            cout << i + 1 << ". Name: " << availableStocks[i].name
                 << ", Price: " << availableStocks[i].price
                 << ", Quantity: " << availableStocks[i].quantity << "\n";
        }
        cout << "Enter stock number to buy (0 to finish): ";
        cin >> choice;
        if (choice > 0 && choice <= availableStocks.size()) {
            int quantity;
            cout << "Enter quantity to buy: ";
            cin >> quantity;
            if (quantity <= availableStocks[choice - 1].quantity) {
                Stock stock = availableStocks[choice - 1];
                stock.quantity = quantity;
                storePendingStock(stock); // Store the purchased stock in pending
                availableStocks[choice - 1].quantity -= quantity;
            } else {
                cout << "Insufficient stock available.\n";
            }
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    }
    cout << "Purchased Stocks added to pending account.\n";

    }


void storePendingStock(const Stock& stock) {
    string command = "StorePENDINGSTOCKS " + cnic + " " + timestamp + " " + type + " " + to_string(amount);
    // Here, you would send the stock information to your JavaScript application
    // via inter-process communication (IPC), file system, network socket, etc.
    // For simplicity, I'm just printing the stock information here.
    cout << "Storing stock '" << stock.name << "' in pending account...\n";
    cout << "Name: " << stock.name << ", Quantity: " << stock.quantity << ", Price: " << stock.price << endl;
}
    vector<Stock> fetchStockFromServer() const {
        // This function should fetch stocks from the server
        // Simulating with hardcoded values for now
        vector<Stock> stocks = {
            {"StockA", 100, 50.0, true},
            {"StockB", 200, 30.0, true},
            {"StockC", 150, 20.0, true},
            {"StockD", 250, 10.0, true}
        };
        return stocks;
    }
};

// Function to display stars for password input
string inputPassword() {
    string password;
    char ch;

    cout << "Enter password: ";
    while ((ch = _getch()) != '\r') {
        if (ch != 8) { // 8 is ASCII for backspace
            password += ch;
            cout << '*';
        } else if (!password.empty()) {
            password.pop_back();
            cout << "\b \b";
        }
    }
    cout << endl;
    return password;
}

// Function for dealer login
bool dealerLogin(const string &username, const string &password) {
    // Here, you'd validate the username and password against your database.
    // Simulating validation with a simple hardcoded check.
    return (username == "admin" && password == "password123");
}

int main() {
    int choice;
    unique_ptr<User> user;

    cout << "1. Dealer Login\n2. Customer\nEnter your choice: ";
    cin >> choice;

    if (choice == 1) {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        password = inputPassword();

        if (dealerLogin(username, password)) {
            user = make_unique<Dealer>(username, password);
        } else {
            cout << "Invalid login credentials.\n";
            return 1;
        }
    } else 
    if (choice == 2) {
        // For customers, directly create a Customer object without asking for credentials
        user = make_unique<Customer>("customer", "");
    } else {
        cout << "Invalid choice.\n";
        return 1;
    }

    while (true) {
        user->displayMenu();
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 6) break;
        user->handleInput(choice);
    }

    return 0;
}
