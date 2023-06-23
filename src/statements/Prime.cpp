#include "prime.cpp"

#include <cstdio>

int main() {
    for (const auto i : PrimeNumberRange{5000}) {
        printf("%d ", i);
    }
    return 0;
}
