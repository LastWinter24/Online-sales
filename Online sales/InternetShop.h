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
    string transactionType; // "credit" (пополнение) или "debit" (списание)
    string date;
};

// Функция для проверки существования файла
bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

// Функция для загрузки данных пользователей из файла
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

// Функция для сохранения всех пользователей в файл
void saveAllUsers(const string& filename, const unordered_map<string, User>& users) {
    ofstream file(filename);
    for (const auto& pair : users) {
        const User& user = pair.second;
        file << user.login << "," << user.password << "," << user.role << "," << user.balance << endl;
    }
}

// Функция для регистрации нового пользователя или администратора
void registerUser(const string& filename, unordered_map<string, User>& users) {
    string login, password, role;
    double balance = 0.0; // начальный баланс для пользователя

    cout << "Выберите роль пользователя (1 - Пользователь, 2 - Администратор): ";
    int roleChoice;
    cin >> roleChoice;

    if (roleChoice == 1) {
        role = "user";
    }
    else if (roleChoice == 2) {
        role = "admin";
    }
    else {
        cout << "Некорректный выбор роли. Регистрация отменена." << endl;
        return;
    }

    cout << "Введите логин (только буквы латинского алфавита и цифры): ";
    cin >> login;
    cout << "Введите пароль (только буквы латинского алфавита и цифры, не менее 8 символов): ";
    cin >> password;

    User newUser = { login, password, role, balance };
    users[login] = newUser;
    saveAllUsers(filename, users);
    cout << "Пользователь успешно зарегистрирован!" << endl;
}

// Функция для авторизации пользователя
bool loginUser(const unordered_map<string, User>& users, string& loggedInUser) {
    string login, password;

    cout << "Авторизация пользователя" << endl;
    cout << "Введите логин: ";
    cin >> login;
    cout << "Введите пароль: ";
    cin >> password;

    auto it = users.find(login);
    if (it != users.end() && it->second.password == password) {
        cout << "Авторизация успешна!" << endl;
        loggedInUser = login;
        return true;
    }
    else {
        cout << "Ошибка авторизации. Проверьте логин и пароль." << endl;
        return false;
    }
}

// Функция для загрузки данных о товарах из файла
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

// Функция для сохранения всех товаров в файл
void saveAllProducts(const string& filename, const vector<Product>& products) {
    ofstream file(filename);
    for (const auto& product : products) {
        file << product.productCode << "," << product.productName << "," << product.brand << ","
            << product.model << "," << product.specs << "," << product.price << ","
            << product.warrantyPeriodMonths << "," << product.image << endl;
    }
}

// Функция для добавления нового товара
void addProduct(const string& filename, vector<Product>& products) {
    string productCode, productName, brand, model, specs, image;
    double price;
    int warrantyPeriodMonths;

    cout << "Введите код товара (только буквы латинского алфавита и цифры): ";
    cin >> productCode;
    cout << "Введите наименование товара: ";
    cin.ignore(); // Очистка буфера перед getline
    getline(cin, productName);
    cout << "Введите бренд товара: ";
    getline(cin, brand);
    cout << "Введите модель товара: ";
    getline(cin, model);
    cout << "Введите спецификации товара: ";
    getline(cin, specs);
    cout << "Введите цену товара (руб.): ";
    cin >> price;
    cout << "Введите гарантийный срок товара (месяцев): ";
    cin >> warrantyPeriodMonths;


    Product newProduct = { productCode, productName, brand, model, specs, price, warrantyPeriodMonths, image };
    products.push_back(newProduct);
    saveAllProducts(filename, products);
    cout << "Товар успешно добавлен!" << endl;
}

// Функция для удаления товара
void deleteProduct(const string& filename, vector<Product>& products) {
    string productCode;
    cout << "Введите код товара для удаления: ";
    cin >> productCode;

    auto it = find_if(products.begin(), products.end(), [&productCode](const Product& p) {
        return p.productCode == productCode;
        });

    if (it != products.end()) {
        cout << "Вы уверены, что хотите удалить товар? (1 - Да / 2 - Нет): ";
        int confirmation;
        cin >> confirmation;
        if (confirmation == 1) {
            products.erase(it);
            saveAllProducts(filename, products);
            cout << "Товар успешно удален!" << endl;
        }
        else {
            cout << "Удаление отменено." << endl;
        }
    }
    else {
        cout << "Товар с указанным кодом не найден." << endl;
    }
}

// Функция для вывода списка товаров на экран
void displayProducts(const vector<Product>& products) {
    cout << "Список товаров:" << endl;
    for (const auto& product : products) {
        cout << "Код товара: " << product.productCode << endl;
        cout << "Наименование: " << product.productName << endl;
        cout << "Бренд: " << product.brand << endl;
        cout << "Модель: " << product.model << endl;
        cout << "Спецификации: " << product.specs << endl;
        cout << "Цена: " << fixed << setprecision(2) << product.price << " руб." << endl;
        cout << "Гарантийный срок: " << product.warrantyPeriodMonths << " месяцев" << endl;
        cout << "-----------------------" << endl;
    }
}

// Функция для генерации кода заказа
string generateOrderCode() {
    static int orderCounter = 1;
    stringstream ss;
    ss << "order" << setfill('0') << setw(4) << orderCounter++;
    return ss.str();
}

// Функция для получения текущей даты и времени
void getCurrentDateTime(string& date, string& time) {
    time_t now = std::time(0);
    tm* ltm = localtime(&now);

    stringstream dateSS, timeSS;
    dateSS << 1900 + ltm->tm_year << "-" << setfill('0') << setw(2) << 1 + ltm->tm_mon << "-" << setfill('0') << setw(2) << ltm->tm_mday;
    timeSS << setfill('0') << setw(2) << ltm->tm_hour << ":" << setfill('0') << setw(2) << ltm->tm_min << ":" << setfill('0') << setw(2) << ltm->tm_sec;

    date = dateSS.str();
    time = timeSS.str();
}

// Функция для оформления заказа
void placeOrder(const string& filename, vector<Product>& products) {
    string productCode, customerName, customerPhone, orderDate, orderTime;
    int quantity;

    cout << "Введите код товара: ";
    cin >> productCode;
    cout << "Введите количество товара: ";
    cin >> quantity;
    cout << "Введите ваше имя: ";
    cin.ignore(); // Очистка буфера перед getline
    getline(cin, customerName);
    cout << "Введите ваш номер телефона: ";
    getline(cin, customerPhone);

    string orderCode = generateOrderCode();
    getCurrentDateTime(orderDate, orderTime);

    Order newOrder = { orderCode, "", productCode, orderDate, orderTime, quantity, customerName, customerPhone, false };

    ofstream file(filename, ios::app);
    file << newOrder.orderCode << "," << newOrder.storeCode << "," << newOrder.productCode << "," << newOrder.orderDate << ","
        << newOrder.orderTime << "," << newOrder.quantity << "," << newOrder.customerName << "," << newOrder.customerPhone << ","
        << newOrder.confirmation << endl;

    cout << "Заказ успешно оформлен! Ваш код заказа: " << orderCode << endl;
}

// Функция для вывода меню пользователя
void userMenu(const string& loggedInUser, unordered_map<string, User>& users, vector<Product>& products) {
    cout << "Меню пользователя '" << loggedInUser << "':" << endl;
    cout << "1. Просмотр товаров" << endl;
    cout << "2. Оформление заказа" << endl;
    cout << "3. Баланс" << endl;
    cout << "4. Пополнение баланса" << endl;
    cout << "5. Выход" << endl;
}

// Функция для вывода меню администратора
void adminMenu(const string& loggedInUser, vector<Product>& products, unordered_map<string, User>& users) {
    cout << "Меню администратора '" << loggedInUser << "':" << endl;
    cout << "1. Просмотр товаров" << endl;
    cout << "2. Добавить товар" << endl;
    cout << "3. Удалить товар" << endl;
    cout << "4. Просмотр пользователей" << endl;
    cout << "5. Удаление пользователей" << endl;
    cout << "6. Баланс" << endl;
    cout << "7. Выход" << endl;
}

// Функция для вывода списка пользователей
void displayUsers(const unordered_map<string, User>& users) {
    cout << "Список зарегистрированных пользователей:" << endl;
    for (const auto& pair : users) {
        const User& user = pair.second;
        cout << "Логин: " << user.login << endl;
        cout << "Роль: " << user.role << endl;
        cout << "Баланс: " << fixed << setprecision(2) << user.balance << " руб." << endl;
        cout << "-----------------------" << endl;
    }
}

// Функция для удаления пользователя
void deleteUser(unordered_map<string, User>& users) {
    string loginToDelete;
    cout << "Введите логин пользователя для удаления: ";
    cin >> loginToDelete;

    auto it = users.find(loginToDelete);
    if (it != users.end()) {
        users.erase(it);
        saveAllUsers(USERS_FILENAME, users);
        cout << "Пользователь успешно удален!" << endl;
    }
    else {
        cout << "Пользователь с указанным логином не найден." << endl;
    }
}

// Функция для записи транзакции в файл
void recordTransaction(const string& filename, const Transaction& transaction) {
    ofstream file(filename, ios::app);
    file << transaction.userLogin << "," << transaction.amount << "," << transaction.transactionType << "," << transaction.date << endl;
}

// Функция для пополнения баланса пользователя
void topUpBalance(const string& loggedInUser, unordered_map<string, User>& users) {
    double amount;
    cout << "Введите сумму для пополнения баланса: ";
    cin >> amount;

    auto it = users.find(loggedInUser);
    if (it != users.end()) {
        it->second.balance += amount;
        saveAllUsers(USERS_FILENAME, users);

        // Запись транзакции
        Transaction transaction = { loggedInUser, amount, "credit", "CURRENT_TIMESTAMP" };
        recordTransaction(TRANSACTIONS_FILENAME, transaction);

        cout << "Баланс успешно пополнен!" << endl;
        cout << "Текущий баланс: " << fixed << setprecision(2) << it->second.balance << " руб." << endl;
    }
    else {
        cout << "Пользователь не найден." << endl;
    }
}

// Функция для вывода баланса пользователя
void displayBalance(const string& loggedInUser, unordered_map<string, User>& users) {
    auto it = users.find(loggedInUser);
    if (it != users.end()) {
        cout << "Текущий баланс: " << fixed << setprecision(2) << it->second.balance << " руб." << endl;
    }
    else {
        cout << "Пользователь не найден." << endl;
    }
}