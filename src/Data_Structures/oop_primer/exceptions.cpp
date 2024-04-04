#include <iostream>
#include <string>

using namespace std;

double sqrt(int num) noexcept(false) {
    if (num < 0) {
        throw std::domain_error{"Cannot handle negative numbers"};
    }
    return num * 1 / 2;
};

int main(int argc, char *argv[]) { return 0; }
