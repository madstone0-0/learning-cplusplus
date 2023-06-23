#include <cstdio>
#include <cstdlib>

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

// struct Account {
//     Account() : id{rand() % 9000}, balance{0} {}
//     Account(long id, double balance) : id{id}, balance{balance} {}
//
//     void deposit(double amount) { balance += amount; }
//
//     void withdraw(double amount) { balance -= amount; }
//
//     long id;
//
//    private:
//     double balance;
// };

struct Account {
    virtual ~Account() = default;

    virtual void deposit(double amount) = 0;
    virtual void withdraw(double amount) = 0;
    virtual long getId() = 0;
};

struct CheckingAccount : Account {
    CheckingAccount() : id{rand() % 9000}, balance{0} {}
    CheckingAccount(long id, double balance) : id{id}, balance{balance} {}

    void deposit(double amount) override { balance += amount; }

    void withdraw(double amount) override { balance -= amount; }

    long getId() override { return id; }

   private:
    long id;
    double balance;
};

struct SavingsAccount : Account {
    SavingsAccount() : id{rand() % 9000}, balance{0} {}
    SavingsAccount(long id, double balance) : id{id}, balance{balance} {}

    void deposit(double amount) override { balance += amount; }

    void withdraw(double amount) override { balance -= amount; }

    long getId() override { return id; }

   private:
    long id;
    double balance;
};

template <typename T>
struct Bank {
    void set_logger(Logger* logger) { this->logger = logger; }

    void make_transfer(T& from, T& to, double amount) {
        logger->log_transfer(from.getId(), to.getId(), amount);
        from.withdraw(amount);
        to.deposit(amount);
    }

   private:
    Logger* logger;
};

int main() {
    srand(42);
    ConsoleLogger con{"baller"};
    FileLogger file;
    // Account accounts[3];
    CheckingAccount martinC{900, 500};
    SavingsAccount martinS{900, 4000};
    CheckingAccount agnes{792, 100};
    Bank<Account> bank;
    bank.set_logger(&con);
    bank.make_transfer(martinC, martinS, 500);
    bank.set_logger(&file);
    bank.make_transfer(martinS, agnes, 600);
    bank.set_logger(&con);
    bank.make_transfer(martinS, martinC, 400);
    return 0;
}
