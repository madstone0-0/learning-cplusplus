#include <cstdio>
#include <vector>

#include "fib.hpp"

int main() {
    std::vector<unsigned long> fibVec{};
    printf("fibVec capacity: %zu\n", fibVec.capacity());
    fibVec.reserve(10);
    for (const auto i : FibonacciRange(4182)) {
        fibVec.push_back(i);
    }
    printf("fibVec capacity: %zu\n", fibVec.capacity());

    for (const auto i : fibVec) {
        printf("%zu ", i);
    }
    printf("\n");

    return 0;
}
