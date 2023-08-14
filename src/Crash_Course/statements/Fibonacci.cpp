#include <cstdio>

#include "fib.hpp"

int main() {
    for (const auto i : FibonacciRange{5000}) {
        printf("%d ", i);
    }
    return 0;
}
