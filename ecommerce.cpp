#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

void clearScreen() {
    system("cls");
}

void adminPanel();
void addItem();
void editItem();
void deleteItem();
void initializeDatabase();
void userPanel();
void browseItems();
void addToCart(int itemId);
void viewCart();
void checkout();
void viewItemDescription(int itemId);

vector<int> cart;
const string ADMIN_PASSWORD = "admin123"; // Admin password

int main() {
    initializeDatabase();
    userPanel();
    return 0;
}

void initializeDatabase() {
    sqlite3 *db;
    char *errMsg = 0;
    int rc = sqlite3_open("ecommerce.db", &db);

    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    } else {
        cout << "Opened database successfully\n";
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS ITEMS("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "NAME TEXT NOT NULL,"
                      "PRICE REAL,"
                      "DESCRIPTION TEXT);";

    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Table created successfully\n";
    }
    sqlite3_close(db);
}

void adminPanel() {
    int choice;
    while (true) {
        clearScreen();
        cout << "==================== Admin Panel ====================\n";
        cout << "1. Add Item\n";
        cout << "2. Edit Item\n";
        cout << "3. Delete Item\n";
        cout << "4. Back to Main Menu\n";
        cout << "=====================================================\n";
        cout << "Enter your choice: ";
        cin >> choice;
        

        switch (choice) {
            case 1:
                addItem();
                break;
            case 2:
                editItem();
                break;
            case 3:
                deleteItem();
                break;
            case 4:
                return;
            default:
                cout << "Invalid choice!\n";
        }
    }
}

void addItem() {
    sqlite3 *db;
    char *errMsg = 0;
    int rc = sqlite3_open("ecommerce.db", &db);

    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    string name, description;
    double price;
    cout << "Enter item name: ";
    getline(cin, name);
    cout << "Enter item price: ";
    cin >> price;
    cin.ignore(); // Add this line to ignore the newline character
    cout << "Enter item description: ";
    getline(cin, description);

    stringstream ss;
    ss << "INSERT INTO ITEMS (NAME, PRICE, DESCRIPTION) VALUES ('" << name << "', " << price << ", '" << description << "');";
    string sql = ss.str();
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Item added successfully\n";
    }
    sqlite3_close(db);
}

void editItem() {
    sqlite3 *db;
    char *errMsg = 0;
    int rc = sqlite3_open("ecommerce.db", &db);

    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    int id;
    string name, description;
    double price;
    cout << "Enter item ID to edit: ";
    cin >> id;
    cin.ignore(); // Add this line to ignore the newline character
    cout << "Enter new item name: ";
    getline(cin, name);
    cout << "Enter new item price: ";
    cin >> price;
    cin.ignore(); // Add this line to ignore the newline character
    cout << "Enter new item description: ";
    getline(cin, description);

    stringstream ss;
    ss << "UPDATE ITEMS SET NAME = '" << name << "', PRICE = " << price << ", DESCRIPTION = '" << description << "' WHERE ID = " << id << ";";
    string sql = ss.str();
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Item updated successfully\n";
    }
    sqlite3_close(db);
}

void deleteItem() {
    sqlite3 *db;
    char *errMsg = 0;
    int rc = sqlite3_open("ecommerce.db", &db);

    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    int id;
    cout << "Enter item ID to delete: ";
    cin >> id;
    

    stringstream ss;
    ss << "DELETE FROM ITEMS WHERE ID = " << id << ";";
    string sql = ss.str();
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "Item deleted successfully\n";
    }
    sqlite3_close(db);
}

void userPanel() {
    while (true) {
        clearScreen();
        cout << "==================== User Panel ====================\n";
        cout << "1. Browse Items\n";
        cout << "2. View Cart\n";
        cout << "3. Exit\n";
        cout << "====================================================\n";
        cout << "Enter your choice (or type special word to access admin panel): ";
        string choice;
        cin >> choice;
        

        if (choice == "1") {
            browseItems();
        } else if (choice == "2") {
            viewCart();
        } else if (choice == "3") {
            cout << "Goodbye!\n";
            return;
        } else if (choice == "admin") {
            string password;
            cout << "Enter admin password: ";
            cin >> password;
            
            if (password == ADMIN_PASSWORD) {
                adminPanel();
            } else {
                cout << "Incorrect password!\n";
            }
        } else {
            cout << "Invalid choice!\n";
        }
    }
}

void browseItems() {
    clearScreen();
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("ecommerce.db", &db);

    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    const char *sql = "SELECT ID, NAME, PRICE FROM ITEMS;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    cout << "==================== Available Items ====================\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *name = sqlite3_column_text(stmt, 1);
        double price = sqlite3_column_double(stmt, 2);
        cout << "ID: " << id << ", Name: " << name << ", Price: " << price << "\n";
    }
    cout << "=========================================================\n";

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    int itemId;
    cout << "Enter item ID to view description or add to cart (0 to exit): ";
    cin >> itemId;
    
    if (itemId != 0) {
        int action;
        cout << "1. View Description\n";
        cout << "2. Add to Cart\n";
        cout << "Enter your choice: ";
        cin >> action;
        
        if (action == 1) {
            viewItemDescription(itemId);
        } else if (action == 2) {
            addToCart(itemId);
            browseItems();
        } else {
            cout << "Invalid choice!\n";
        }
    }
}

void viewItemDescription(int itemId) {
    clearScreen();
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("ecommerce.db", &db);

    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    stringstream ss;
    ss << "SELECT DESCRIPTION FROM ITEMS WHERE ID = " << itemId << ";";
    string sql = ss.str();
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *description = sqlite3_column_text(stmt, 0);
        cout << "Description: " << description << "\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    int choice;
    cout << "Do you want to add this item to the cart? (1 for Yes, 0 for No): ";
    cin >> choice;
    
    if (choice == 1) {
        addToCart(itemId);
    }
    browseItems(); // Return to browse items menu
}

void addToCart(int itemId) {
    cart.push_back(itemId);
    cout << "Item added to cart\n";
}

void viewCart() {
    clearScreen();
    if (cart.empty()) {
        cout << "Your cart is empty\n";
        cout << "Press Enter to return to the User Panel...";
        cin.ignore();
        cin.get();
        return;
    }

    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("ecommerce.db", &db);

    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    double total = 0;
    cout << "==================== Items in Your Cart ====================\n";
    for (size_t i = 0; i < cart.size(); ++i) { 
        int itemId = cart[i];
        stringstream ss;
        ss << "SELECT NAME, PRICE FROM ITEMS WHERE ID = " << itemId << ";";
        string sql = ss.str();
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

        if (rc != SQLITE_OK) {
            cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << endl;
            sqlite3_close(db);
            return;
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char *name = sqlite3_column_text(stmt, 0);
            double price = sqlite3_column_double(stmt, 1);
            total += price;
            cout << "Name: " << name << ", Price: " << price << "\n";
        }

        sqlite3_finalize(stmt);
    }
    cout << "============================================================\n";
    cout << "Total Amount: " << total << "\n";
    cout << "============================================================\n";

    sqlite3_close(db);

    int choice;
    cout << "1. Checkout\n";
    cout << "2. Back to User Panel\n";
    cout << "Enter your choice: ";
    cin >> choice;
    
    if (choice == 1) {
        checkout();
    }
}

void checkout() {
    clearScreen();
    cout << "==================== Checkout ====================\n";
    cout << "Choose payment method:\n";
    cout << "1. Card\n";
    cout << "2. Cash on Delivery\n";
    cout << "==================================================\n";
    int choice;
    cout << "Enter your choice: ";
    cin >> choice;
    

    if (choice == 1) {
        cout << "Payment successful via Card!\n";
    } else if (choice == 2) {
        cout << "Payment will be made on delivery!\n";
    } else {
        cout << "Invalid choice!\n";
    }

    cart.clear();
    cout << "Thank you for your purchase!\n";
    cout << "Press Enter to return to the User Panel...";
    cin.ignore();
    cin.get();
    userPanel();
}