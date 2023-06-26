#include <cstdio>
#include <map>
#include <stdexcept>

/* Naive Implementation */
// struct ConsoleLogger {
//     void log_transfer(long from, long to, double amount) {
//         printf("[con] %ld -> %ld: %f\n", from, to, amount);
//     }
// };
//
// struct FileLogger {
//     void log_transfer(long from, long to, double amount) {
//         printf("[file] %ld -> %ld: %f\n", from, to, amount);
//     }
// };
//
// enum class LoggerType {
//     Console,
//     File,
// };
//
// struct Bank {
//     Bank() : type{LoggerType::Console} {}
//     void set_logger(LoggerType new_type) { type = new_type; }
//
//     void make_transfer(long from, long to, double amount) {
//         switch (type) {
//             case LoggerType::Console: {
//                 consoleLogger.log_transfer(from, to, amount);
//                 break;
//             }
//             case LoggerType::File: {
//                 fileLogger.log_transfer(from, to, amount);
//                 break;
//             }
//             default: {
//                 throw std::logic_error("Unknown Logger type encountered");
//             }
//         }
//     }
//
//    private:
//     LoggerType type;
//     ConsoleLogger consoleLogger;
//     FileLogger fileLogger;
// };
/* Naive Implementation */

/* Inheritance Primer */
// Virtual Methods
// // struct BaseClass {
// //     virtual const char* final_message() { return "Balla"; }
// // };
// //
// // struct DerivedClass : BaseClass {
// //     const char* final_message() override { return "SHADOW WIZARD MONEY
// GANG";
// //     }
// // };
//
// struct BaseClass {
//     virtual const char* final_message() const = 0;
// };
//
// struct DerivedClass : BaseClass {
//     const char* final_message() const override { return "Overridden"; }
// };
//
// int main() {
//     // BaseClass base;
//     DerivedClass derived;
//     BaseClass& ref = derived;
//     // printf("BaseClass: %s\n", base.final_message());
//     printf("DerivedClass: %s\n", derived.final_message());
//     printf("BaseClass&: %s\n", ref.final_message());
// }

// Pure-Virtual Classes and Virtual Destructors
// struct BaseClass {
//     virtual ~BaseClass() = default;
// };
//
// struct DerivedClass : BaseClass {
//     DerivedClass() { printf("DerivedClass() invoked\n"); }
//
//     ~DerivedClass() { printf("~DerivedClass() invoked\n"); }
// };
//
// int main() {
//     printf("Constructing DerivedClass x\n");
//     BaseClass* x{new DerivedClass{}};
//     printf("Deleting x as a BaseClass\n");
//     delete x;
// }
/* Inheritance Primer */

/* Implementation Inheritance */
// struct Logger {
//     virtual ~Logger() = default;
//     virtual void log_transfer(long from, long to, double amount) = 0;
// };
//
// struct ConsoleLogger : Logger {
//     void log_transfer(long from, long to, double amount) override {
//         printf("[cons] %ld -> %ld: %f\n", from, to, amount);
//     }
// };
//
// struct FileLogger : Logger {
//     void log_transfer(long from, long to, double amount) override {
//         printf("[file] %ld -> %ld: %f\n", from, to, amount);
//     }
// };

// Constructor Injection
// struct Bank {
//     Bank(Logger& logger) : logger{logger} {}
//     void make_transfer(long from, long to, double amount) {
//         logger.log_transfer(from, to, amount);
//     }
//
//    private:
//     Logger& logger;
// };
//
// int main() {
//     ConsoleLogger logger;
//     Bank bank{logger};
//     bank.make_transfer(1000, 2000, 49.95);
//     bank.make_transfer(2000, 4000, 20.00);
// }

// Property Injection
// struct Bank {
//     void set_logger(Logger* logger) { this->logger = logger; }
//
//     void make_transfer(long from, long to, double amount) {
//         logger->log_transfer(from, to, amount);
//     }
//
//    private:
//     Logger* logger{};
// };

// Both
// struct Bank {
//     Bank(Logger* logger) : logger{logger} {}
//
//     void set_logger(Logger* logger) { this->logger = logger; }
//
//     void make_transfer(long from, long to, double amount) {
//         logger->log_transfer(from, to, amount);
//     }
//
//    private:
//     Logger* logger;
// };
//
//
// int main() {
//     ConsoleLogger con;
//     FileLogger file;
//     Bank bank{&con};
//     bank.make_transfer(1000, 2000, 500);
//     bank.set_logger(&file);
//     bank.make_transfer(1000, 2000, 600);
//     bank.set_logger(&con);
//     bank.make_transfer(5000, 2000, 400);
//     return 0;
// }

/* Implementation Inheritance */

/* Exercises */

struct Logger {
    virtual ~Logger() = default;
    virtual void log_transfer(long from, long to, double amount) = 0;
};

struct ConsoleLogger : Logger {
    ConsoleLogger(const char* suffix) : suffix{suffix} {}
    void log_transfer(long from, long to, double amount) override {
        printf("[cons] %ld -> %ld: %f %s\n", from, to, amount, suffix);
    }

   private:
    const char* suffix;
};

struct FileLogger : Logger {
    void log_transfer(long from, long to, double amount) override {
        printf("[file] %ld -> %ld: %f\n", from, to, amount);
    }
};

// AccountDatabase
struct AccountDatabase {
    ~AccountDatabase() = default;

    virtual void set_balance(long id, double amount) = 0;
    virtual double get_balance(long id) = 0;

    virtual std::map<long, double> get_accounts() = 0;
};

struct InMemoryAccountDatabase : AccountDatabase {
    void set_balance(long id, double amount) override { accounts[id] = amount; }

    double get_balance(long id) override { return accounts[id]; }

    std::map<long, double> get_accounts() override { return accounts; }

   private:
    std::map<long, double> accounts;
};

// Bank
struct Bank {
    Bank(AccountDatabase& database) : database{database} {}

    void set_logger(Logger* logger) { this->logger = logger; }

    void make_transfer(long from, long to, double amount) {
        logger->log_transfer(from, to, amount);
        database.set_balance(from, (database.get_balance(from) - amount));
        database.set_balance(to, (database.get_balance(to) + amount));
    }

    void show() {
        printf("\nAccounts\n");
        for (auto account : database.get_accounts()) {
            printf("[%ld]:\t\t%f\n", account.first, account.second);
        }
    }

   private:
    Logger* logger;
    AccountDatabase& database;
};

int main() {
    ConsoleLogger logger{"baller"};
    InMemoryAccountDatabase database;
    Bank bank{database};
    bank.set_logger(&logger);
    bank.make_transfer(1000, 5000, 400);
    bank.make_transfer(5000, 1000, 5000);
    bank.make_transfer(2000, 5000, -800);
    bank.make_transfer(3000, 1000, 100);

    bank.show();

    return 0;
}

/* Exercises */
