#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>

class InvalidBalanceException : public std::runtime_error {
public:
    InvalidBalanceException() : std::runtime_error("Недопустимый баланс!") {}
};

// Базовый класс User
class User {
protected:
    int id;
    std::string name;
    float balance;

public:
    User(int id, const std::string& name, float balance)
        : id(id), name(name) {
        if (balance < 0) {
            throw InvalidBalanceException();
        }
        this->balance = balance;
    }

    virtual void displayInfo() const {
        std::cout << "ID: " << id << ", Имя: " << name << ", Баланс: " << balance;
    }

    virtual bool isVIP() const { return false; }

    int getId() const { return id; }
    const std::string& getName() const { return name; }
    float getBalance() const { return balance; }

    virtual ~User() = default;
};

// Производный класс VIPUser
class VIPUser : public User {
private:
    float cashbackRate;

public:
    VIPUser(int id, const std::string& name, float balance, float cashbackRate)
        : User(id, name, balance), cashbackRate(cashbackRate) {}

    void displayInfo() const override {
        User::displayInfo();
        std::cout << ", Кэшбэк: " << cashbackRate * 100 << "%";
    }

    bool isVIP() const override { return true; }

    void addCashback() {
        balance += balance * cashbackRate;
    }
};

// Функция для сортировки пользователей по статусу
void sortUsersByStatus(std::vector<std::shared_ptr<User>>& users) {
    std::sort(users.begin(), users.end(), [](const std::shared_ptr<User>& a, const std::shared_ptr<User>& b) {
        return a->isVIP() > b->isVIP(); // VIP-пользователи идут первыми
        });
}

// Функция для поиска пользователя по ID
std::shared_ptr<User> findUserById(const std::vector<std::shared_ptr<User>>& users, int id) {
    auto it = std::find_if(users.begin(), users.end(), [id](const std::shared_ptr<User>& user) {
        return user->getId() == id;
        });

    if (it != users.end()) {
        return *it;
    }

    return nullptr;
}

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        auto user1 = std::make_shared<User>(1, "Анна", 1200.0f);
        auto user2 = std::make_shared<User>(2, "Иван", 800.0f);
        auto vip1 = std::make_shared<VIPUser>(3, "Мария", 5000.0f, 0.05f);
        auto vip2 = std::make_shared<VIPUser>(4, "Петр", 3000.0f, 0.1f);

        // Контейнер для хранения пользователей
        std::vector<std::shared_ptr<User>> allUsers = { user1, user2, vip1, vip2 };

        // Сортировка пользователей по статусу
        sortUsersByStatus(allUsers);

        // Выводим отсортированных пользователей
        std::cout << "Пользователи после сортировки по статусу (VIP -> обычные):\n";
        for (const auto& user : allUsers) {
            user->displayInfo();
            std::cout << std::endl;
        }

        // Интерактивный поиск по ID
        while (true) {
            int searchId;
            std::cout << "\nВведите ID для поиска пользователя (или введите 0 для выхода): ";
            std::cin >> searchId;

            if (searchId == 0) {
                std::cout << "Выход из программы.\n";
                break;
            }

            auto foundUser = findUserById(allUsers, searchId);
            if (foundUser) {
                std::cout << "\nНайден пользователь:\n";
                foundUser->displayInfo();
                std::cout << std::endl;
            }
            else {
                std::cout << "Пользователь с ID " << searchId << " не найден\n";
            }
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
