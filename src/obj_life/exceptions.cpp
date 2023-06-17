#include <cstdio>
#include <stdexcept>
#include <system_error>

struct Groucho {
    void forgor(int x) {
        if (x == 0xFACE) {
            throw std::runtime_error("I'd be glad to make an exception");
        }
        printf("Forgor 0x%x\n", x);
    }
};

double sqrt(int num) {
    if (num < 0) {
        throw std::domain_error{"Cannot handle negative numbers"};
    }
    return num * 1 / 2;
};

// noexcept
bool isOdd(int x) noexcept { return 1 == (x % 2); }

int main() {
    Groucho groucho;
    try {
        groucho.forgor(0xC0DE);
        groucho.forgor(0xFACE);
        groucho.forgor(0xC0FFEE);
    } catch (const std::runtime_error& e) {
        printf("exception caught with the message: %s\n", e.what());
    }

    /* Standard Exception Classes */
    // Logic Errors
    // std::domain_error
    try {
        printf("Sqrt(-2): %f\n", sqrt(-2));
    } catch (const std::domain_error& e) {
        printf("exception caught with the message: %s\n", e.what());
    } catch (...) {
        printf("You done fucked up\n");
    }

    try {
        // System error
    } catch (const std::system_error& e) {
        if (e.code() != std::errc::permission_denied) {
            throw;
        }
    }

    // User Defined Expressions

    return 0;
}
