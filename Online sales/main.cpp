#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string> 
#include "InternetShop.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "RU");

    unordered_map<string, User> users;
    vector<Product> products;

    // Загрузка данных из файлов
    if (fileExists(USERS_FILENAME)) {
        users = loadUsers(USERS_FILENAME);
    }
    if (fileExists(PRODUCTS_FILENAME)) {
        products = loadProducts(PRODUCTS_FILENAME);
    }

    string loggedInUser;
    bool loggedIn = false;

    while (true) {
        cout << "Добро пожаловать!" << endl;
        cout << "1. Регистрация" << endl;
        cout << "2. Авторизация" << endl;
        cout << "3. Выход" << endl;
        int choice;
        cin >> choice;

        if (choice == 1) {
            registerUser(USERS_FILENAME, users);
        }
        else if (choice == 2) {
            loggedIn = loginUser(users, loggedInUser);
            if (!loggedIn) {
                continue;
            }
        }
        else if (choice == 3) {
            cout << "До свидания!" << endl;
            break;
        }
        else {
            cout << "Некорректный выбор. Попробуйте снова." << endl;
            continue;
        }

        while (loggedIn) {
            if (users[loggedInUser].role == "user") {
                userMenu(loggedInUser, users, products);
                int userChoice;
                cin >> userChoice;

                switch (userChoice) {
                case 1:
                    displayProducts(products);
                    break;
                case 2:
                    placeOrder(ORDERS_FILENAME, products);
                    break;
                case 3:
                    displayBalance(loggedInUser, users);
                    break;
                case 4:
                    topUpBalance(loggedInUser, users);
                    break;
                case 5:
                    loggedIn = false; // Выход из учетной записи пользователя
                    cout << "Выход выполнен." << endl;
                    break;
                default:
                    cout << "Некорректный выбор. Попробуйте снова." << endl;
                    break;
                }
            }
            else if (users[loggedInUser].role == "admin") {
                adminMenu(loggedInUser, products, users);
                int adminChoice;
                cin >> adminChoice;

                switch (adminChoice) {
                case 1:
                    displayProducts(products);
                    break;
                case 2:
                    addProduct(PRODUCTS_FILENAME, products);
                    break;
                case 3:
                    deleteProduct(PRODUCTS_FILENAME, products);
                    break;
                case 4:
                    displayUsers(users);
                    break;
                case 5:
                    deleteUser(users);
                    break;
                case 6:
                    displayBalance(loggedInUser, users);
                    break;
                case 7:
                    loggedIn = false; // Выход из учетной записи администратора
                    cout << "Выход выполнен." << endl;
                    break;
                default:
                    cout << "Некорректный выбор. Попробуйте снова." << endl;
                    break;
                }
            }
        }
    }


	return 0;
}



