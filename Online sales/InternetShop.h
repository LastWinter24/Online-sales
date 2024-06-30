#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>  // For current date and time

using namespace std;

const string USERS_FILENAME = "users.csv";
const string PRODUCTS_FILENAME = "products.csv";
const string ORDERS_FILENAME = "orders.csv";
const string TRANSACTIONS_FILENAME = "transactions.csv";

struct User {
    string login;
    string password;
    string role;
    double balance;
};

struct Product {
    string productCode;
    string productName;
    string brand;
    string model;
    string specs;
    double price;
    int warrantyPeriodMonths;
    string image;
};

struct Order {
    string orderCode;
    string storeCode;
    string productCode;
    string orderDate;
    string orderTime;
    int quantity;
    string customerName;
    string customerPhone;
    bool confirmation;
};

struct Transaction {
    string userLogin;
    double amount;
    string transactionType; // "credit" (����������) ��� "debit" (��������)
    string date;
};

// ������� ��� �������� ������������� �����
bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

// ������� ��� �������� ������ ������������� �� �����
unordered_map<string, User> loadUsers(const string& filename) {
    unordered_map<string, User> users;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string login, password, role;
        double balance;
        getline(ss, login, ',');
        getline(ss, password, ',');
        getline(ss, role, ',');
        ss >> balance;
        ss.ignore();

        User user = { login, password, role, balance };
        users[login] = user;
    }

    return users;
}

// ������� ��� ���������� ���� ������������� � ����
void saveAllUsers(const string& filename, const unordered_map<string, User>& users) {
    ofstream file(filename);
    for (const auto& pair : users) {
        const User& user = pair.second;
        file << user.login << "," << user.password << "," << user.role << "," << user.balance << endl;
    }
}

// ������� ��� ����������� ������ ������������ ��� ��������������
void registerUser(const string& filename, unordered_map<string, User>& users) {
    string login, password, role;
    double balance = 0.0; // ��������� ������ ��� ������������

    cout << "�������� ���� ������������ (1 - ������������, 2 - �������������): ";
    int roleChoice;
    cin >> roleChoice;

    if (roleChoice == 1) {
        role = "user";
    }
    else if (roleChoice == 2) {
        role = "admin";
    }
    else {
        cout << "������������ ����� ����. ����������� ��������." << endl;
        return;
    }

    cout << "������� ����� (������ ����� ���������� �������� � �����): ";
    cin >> login;
    cout << "������� ������ (������ ����� ���������� �������� � �����, �� ����� 8 ��������): ";
    cin >> password;

    User newUser = { login, password, role, balance };
    users[login] = newUser;
    saveAllUsers(filename, users);
    cout << "������������ ������� ���������������!" << endl;
}

// ������� ��� ����������� ������������
bool loginUser(const unordered_map<string, User>& users, string& loggedInUser) {
    string login, password;

    cout << "����������� ������������" << endl;
    cout << "������� �����: ";
    cin >> login;
    cout << "������� ������: ";
    cin >> password;

    auto it = users.find(login);
    if (it != users.end() && it->second.password == password) {
        cout << "����������� �������!" << endl;
        loggedInUser = login;
        return true;
    }
    else {
        cout << "������ �����������. ��������� ����� � ������." << endl;
        return false;
    }
}

// ������� ��� �������� ������ � ������� �� �����
vector<Product> loadProducts(const string& filename) {
    vector<Product> products;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string productCode, productName, brand, model, specs, image;
        double price;
        int warrantyPeriodMonths;

        getline(ss, productCode, ',');
        getline(ss, productName, ',');
        getline(ss, brand, ',');
        getline(ss, model, ',');
        getline(ss, specs, ',');
        ss >> price;
        ss.ignore();
        ss >> warrantyPeriodMonths;
        ss.ignore();
        getline(ss, image, ',');

        Product product = { productCode, productName, brand, model, specs, price, warrantyPeriodMonths, image };
        products.push_back(product);
    }

    return products;
}

// ������� ��� ���������� ���� ������� � ����
void saveAllProducts(const string& filename, const vector<Product>& products) {
    ofstream file(filename);
    for (const auto& product : products) {
        file << product.productCode << "," << product.productName << "," << product.brand << ","
            << product.model << "," << product.specs << "," << product.price << ","
            << product.warrantyPeriodMonths << "," << product.image << endl;
    }
}

// ������� ��� ���������� ������ ������
void addProduct(const string& filename, vector<Product>& products) {
    string productCode, productName, brand, model, specs, image;
    double price;
    int warrantyPeriodMonths;

    cout << "������� ��� ������ (������ ����� ���������� �������� � �����): ";
    cin >> productCode;
    cout << "������� ������������ ������: ";
    cin.ignore(); // ������� ������ ����� getline
    getline(cin, productName);
    cout << "������� ����� ������: ";
    getline(cin, brand);
    cout << "������� ������ ������: ";
    getline(cin, model);
    cout << "������� ������������ ������: ";
    getline(cin, specs);
    cout << "������� ���� ������ (���.): ";
    cin >> price;
    cout << "������� ����������� ���� ������ (�������): ";
    cin >> warrantyPeriodMonths;


    Product newProduct = { productCode, productName, brand, model, specs, price, warrantyPeriodMonths, image };
    products.push_back(newProduct);
    saveAllProducts(filename, products);
    cout << "����� ������� ��������!" << endl;
}

// ������� ��� �������� ������
void deleteProduct(const string& filename, vector<Product>& products) {
    string productCode;
    cout << "������� ��� ������ ��� ��������: ";
    cin >> productCode;

    auto it = find_if(products.begin(), products.end(), [&productCode](const Product& p) {
        return p.productCode == productCode;
        });

    if (it != products.end()) {
        cout << "�� �������, ��� ������ ������� �����? (1 - �� / 2 - ���): ";
        int confirmation;
        cin >> confirmation;
        if (confirmation == 1) {
            products.erase(it);
            saveAllProducts(filename, products);
            cout << "����� ������� ������!" << endl;
        }
        else {
            cout << "�������� ��������." << endl;
        }
    }
    else {
        cout << "����� � ��������� ����� �� ������." << endl;
    }
}

// ������� ��� ������ ������ ������� �� �����
void displayProducts(const vector<Product>& products) {
    cout << "������ �������:" << endl;
    for (const auto& product : products) {
        cout << "��� ������: " << product.productCode << endl;
        cout << "������������: " << product.productName << endl;
        cout << "�����: " << product.brand << endl;
        cout << "������: " << product.model << endl;
        cout << "������������: " << product.specs << endl;
        cout << "����: " << fixed << setprecision(2) << product.price << " ���." << endl;
        cout << "����������� ����: " << product.warrantyPeriodMonths << " �������" << endl;
        cout << "-----------------------" << endl;
    }
}

// ������� ��� ��������� ���� ������
string generateOrderCode() {
    static int orderCounter = 1;
    stringstream ss;
    ss << "order" << setfill('0') << setw(4) << orderCounter++;
    return ss.str();
}

// ������� ��� ��������� ������� ���� � �������
void getCurrentDateTime(string& date, string& time) {
    time_t now = std::time(0);
    tm* ltm = localtime(&now);

    stringstream dateSS, timeSS;
    dateSS << 1900 + ltm->tm_year << "-" << setfill('0') << setw(2) << 1 + ltm->tm_mon << "-" << setfill('0') << setw(2) << ltm->tm_mday;
    timeSS << setfill('0') << setw(2) << ltm->tm_hour << ":" << setfill('0') << setw(2) << ltm->tm_min << ":" << setfill('0') << setw(2) << ltm->tm_sec;

    date = dateSS.str();
    time = timeSS.str();
}

// ������� ��� ���������� ������
void placeOrder(const string& filename, vector<Product>& products) {
    string productCode, customerName, customerPhone, orderDate, orderTime;
    int quantity;

    cout << "������� ��� ������: ";
    cin >> productCode;
    cout << "������� ���������� ������: ";
    cin >> quantity;
    cout << "������� ���� ���: ";
    cin.ignore(); // ������� ������ ����� getline
    getline(cin, customerName);
    cout << "������� ��� ����� ��������: ";
    getline(cin, customerPhone);

    string orderCode = generateOrderCode();
    getCurrentDateTime(orderDate, orderTime);

    Order newOrder = { orderCode, "", productCode, orderDate, orderTime, quantity, customerName, customerPhone, false };

    ofstream file(filename, ios::app);
    file << newOrder.orderCode << "," << newOrder.storeCode << "," << newOrder.productCode << "," << newOrder.orderDate << ","
        << newOrder.orderTime << "," << newOrder.quantity << "," << newOrder.customerName << "," << newOrder.customerPhone << ","
        << newOrder.confirmation << endl;

    cout << "����� ������� ��������! ��� ��� ������: " << orderCode << endl;
}

// ������� ��� ������ ���� ������������
void userMenu(const string& loggedInUser, unordered_map<string, User>& users, vector<Product>& products) {
    cout << "���� ������������ '" << loggedInUser << "':" << endl;
    cout << "1. �������� �������" << endl;
    cout << "2. ���������� ������" << endl;
    cout << "3. ������" << endl;
    cout << "4. ���������� �������" << endl;
    cout << "5. �����" << endl;
}

// ������� ��� ������ ���� ��������������
void adminMenu(const string& loggedInUser, vector<Product>& products, unordered_map<string, User>& users) {
    cout << "���� �������������� '" << loggedInUser << "':" << endl;
    cout << "1. �������� �������" << endl;
    cout << "2. �������� �����" << endl;
    cout << "3. ������� �����" << endl;
    cout << "4. �������� �������������" << endl;
    cout << "5. �������� �������������" << endl;
    cout << "6. ������" << endl;
    cout << "7. �����" << endl;
}

// ������� ��� ������ ������ �������������
void displayUsers(const unordered_map<string, User>& users) {
    cout << "������ ������������������ �������������:" << endl;
    for (const auto& pair : users) {
        const User& user = pair.second;
        cout << "�����: " << user.login << endl;
        cout << "����: " << user.role << endl;
        cout << "������: " << fixed << setprecision(2) << user.balance << " ���." << endl;
        cout << "-----------------------" << endl;
    }
}

// ������� ��� �������� ������������
void deleteUser(unordered_map<string, User>& users) {
    string loginToDelete;
    cout << "������� ����� ������������ ��� ��������: ";
    cin >> loginToDelete;

    auto it = users.find(loginToDelete);
    if (it != users.end()) {
        users.erase(it);
        saveAllUsers(USERS_FILENAME, users);
        cout << "������������ ������� ������!" << endl;
    }
    else {
        cout << "������������ � ��������� ������� �� ������." << endl;
    }
}

// ������� ��� ������ ���������� � ����
void recordTransaction(const string& filename, const Transaction& transaction) {
    ofstream file(filename, ios::app);
    file << transaction.userLogin << "," << transaction.amount << "," << transaction.transactionType << "," << transaction.date << endl;
}

// ������� ��� ���������� ������� ������������
void topUpBalance(const string& loggedInUser, unordered_map<string, User>& users) {
    double amount;
    cout << "������� ����� ��� ���������� �������: ";
    cin >> amount;

    auto it = users.find(loggedInUser);
    if (it != users.end()) {
        it->second.balance += amount;
        saveAllUsers(USERS_FILENAME, users);

        // ������ ����������
        Transaction transaction = { loggedInUser, amount, "credit", "CURRENT_TIMESTAMP" };
        recordTransaction(TRANSACTIONS_FILENAME, transaction);

        cout << "������ ������� ��������!" << endl;
        cout << "������� ������: " << fixed << setprecision(2) << it->second.balance << " ���." << endl;
    }
    else {
        cout << "������������ �� ������." << endl;
    }
}

// ������� ��� ������ ������� ������������
void displayBalance(const string& loggedInUser, unordered_map<string, User>& users) {
    auto it = users.find(loggedInUser);
    if (it != users.end()) {
        cout << "������� ������: " << fixed << setprecision(2) << it->second.balance << " ���." << endl;
    }
    else {
        cout << "������������ �� ������." << endl;
    }
}