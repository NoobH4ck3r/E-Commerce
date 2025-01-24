#include <iostream>
#include <string>
#include <fstream>
using namespace std;


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



struct Item {
    int id;
    string name;
    double price;
    string description;
};

Item items[100];
int itemCount = 0;
int cart[100];
int cartSize = 0;

int main() {
    initializeDatabase();
    userPanel();
    return 0;
}

void initializeDatabase() {
    ifstream file("ecommerce.txt");
    if (!file) {
        ofstream outfile("ecommerce.txt");
        outfile.close();
        cout<<"Data file created successfully\n";
    } else {
        cout<<"Opened data file successfully\n";
    }
    file.close();
}

void loadItems() {
    ifstream file("ecommerce.txt");
    if (file) {
        itemCount = 0;
        string line;
        while (getline(file, line)) {
            itemCount++;
        }
        itemCount /= 4;

        file.clear();
        file.seekg(0, ios::beg);

        for (int i = 0; i < itemCount; ++i) {
            file>>items[i].id;
            file.ignore();
            getline(file, items[i].name);
            file>>items[i].price;
            file.ignore();
            getline(file, items[i].description);
        }
    }
    file.close();
}

void saveItems() {
    ofstream file("ecommerce.txt");
    for (int i = 0; i < itemCount; ++i) {
        file<<items[i].id<<'\n'<<items[i].name<<'\n'<<items[i].price<<'\n'<<items[i].description<<'\n';
    }
    file.close();
}

void adminPanel() {
    int choice;
    while (true) {
        system("cls");
        cout<<"==================== Admin Panel ====================\n"<<"1. Add Item\n"<<"2. Edit Item\n"<<"3. Delete Item\n"<<"4. Back to Main Menu\n"<<"=====================================================\n"<<"Enter your choice: ";
        cin>>choice;
        cin.ignore();

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
                cout<<"Invalid choice!\n";
        }
    }
}

void addItem() {
    loadItems();
    if (itemCount < 100) {
        if (itemCount == 0) {
            items[itemCount].id = 1;
        } else {
            items[itemCount].id = items[itemCount - 1].id + 1;
        }
        cout<<"Enter item name: ";
        getline(cin, items[itemCount].name);
        cout<<"Enter item price: ";
        cin>>items[itemCount].price;
        cin.ignore();
        cout<<"Enter item description: ";
        getline(cin, items[itemCount].description);

        itemCount++;
        saveItems();
        cout<<"Item added successfully\n";
    } else {
        cout<<"Item list is full\n";
    }
}

void editItem() {
    loadItems();
    int id;
    cout<<"Enter item ID to edit: ";
    cin>>id;
    cin.ignore();

    for (int i = 0; i < itemCount; ++i) {
        if (items[i].id == id) {
            cout<<"Enter new item name: ";
            getline(cin, items[i].name);
            cout<<"Enter new item price: ";
            cin>>items[i].price;
            cin.ignore();
            cout<<"Enter new item description: ";
            getline(cin, items[i].description);

            saveItems();
            cout<<"Item updated successfully\n";
            return;
        }
    }
    cout<<"Item not found\n";
}

void deleteItem() {
    loadItems();
    int id;
    cout<<"Enter item ID to delete: ";
    cin>>id;
    cin.ignore();

    int newCount = 0;
    for (int i=0; i<itemCount; ++i) {
        if (items[i].id != id) {
            items[newCount++] = items[i];
        }
    }
    itemCount = newCount;

    saveItems();
    cout<<"Item deleted successfully\n";
}

void userPanel() {
    while (true) {
        system("cls");
        cout<<"==================== User Panel ====================\n"<<"1. Browse Items\n"<<"2. View Cart\n"<<"3. Exit\n"<<"====================================================\n"<<"Enter your choice (or type special word to access admin panel): ";
        string choice;
        cin>>choice;
        cin.ignore();

        if (choice == "1") {
            browseItems();
        } else if (choice == "2") {
            viewCart();
        } else if (choice == "3") {
            cout<<"Goodbye!\n";
            return;
        } else if (choice == "admin") {
            string password;
            string adminpass = "admin123";
            cout<<"Enter admin password: ";
            cin>>password;
            cin.ignore();

            if (password == adminpass) {
                adminPanel();
            } else {
                cout<<"Incorrect password!\n";
            }
        } else {
            cout<<"Invalid choice!\n";
        }
    }
}

void browseItems() {
    system("cls");
    loadItems();
    cout<<"==================== Available Items ====================\n";
    for (int i = 0; i < itemCount; ++i) {
        cout<<"ID: "<<items[i].id<<", Name: "<<items[i].name<<", Price: "<<items[i].price<<"\n";
    }
    cout<<"=========================================================\n";

    int itemId;
    cout<<"Enter item ID to view description or add to cart (0 to exit): ";
    cin>>itemId;
    cin.ignore();

    if (itemId != 0) {
        int action;
        cout<<"1. View Description\n"<<"2. Add to Cart\n"<<"Enter your choice: ";
        cin>>action;
        cin.ignore();

        if (action == 1) {
            viewItemDescription(itemId);
        } else if (action == 2) {
            addToCart(itemId);
            browseItems();
        } else {
            cout<<"Invalid choice!\n";
        }
    }
}

void viewItemDescription(int itemId) {
    system("cls");
    loadItems();
    for (int i=0; i<itemCount; ++i) {
        if (items[i].id == itemId) {
            cout<<"Description: "<<items[i].description<<"\n";
            break;
        }
    }

    int choice;
    cout<<"Do you want to add this item to the cart? (1 for Yes, 0 for No): ";
    cin>>choice;
    cin.ignore();
    if (choice == 1) {
        addToCart(itemId);
    }
    browseItems();
}

void addToCart(int itemId) {
    if (cartSize < 100) {
        cart[cartSize++] = itemId;
        cout<<"Item added to cart\n";
    } else {
        cout<<"Cart is full\n";
    }
}

void viewCart() {
    system("cls");
    if (cartSize == 0) {
        cout<<"Your cart is empty\n"<<"Press Enter to return to the User Panel...";
        cin.ignore();
        cin.get();
        return;
    }

    loadItems();
    double total = 0;
    cout<<"==================== Items in Your Cart ====================\n";
    for (int i=0; i<cartSize; ++i) {
        for (int j=0; j<itemCount; ++j) {
            if (items[j].id == cart[i]) {
                total += items[j].price;
                cout<<"Name: "<<items[j].name<<", Price: "<<items[j].price<<"\n";
                break;
            }
        }
    }
    cout<<"============================================================\n"<<"Total Amount: "<<total<<"\n"<<"============================================================\n";

    int choice;
    cout<<"1. Checkout\n"<<"2. Back to User Panel\n"<<"Enter your choice: ";
    cin>>choice;
    cin.ignore();

    if (choice == 1) {
        checkout();
    }
}

void checkout() {
    system("cls");
    cout<<"==================== Checkout ====================\n"<<"Choose payment method:\n"<<"1. Card\n"<<"2. Cash on Delivery\n"<<"==================================================\n"<<"Enter your choice: ";
    int choice;
    cin>>choice;
    cin.ignore();

    if (choice == 1) {
        cout<<"Payment successful via Card!\n";
    } else if (choice == 2) {
        cout<<"Payment will be made on delivery!\n";
    } else {
        cout<<"Invalid choice!\n";
    }

    cartSize = 0;
    cout<<"Thank you for your purchase!\n"<<"Press Enter to return to the User Panel...";
    cin.get();
    userPanel();
}