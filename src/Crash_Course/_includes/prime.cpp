#include "prime.hpp"

bool PrimeNumberIterator::operator!=(int x) const { return x >= current; }

PrimeNumberIterator& PrimeNumberIterator::operator++() {
    for (auto i = current + 1;; i++) {
        auto is_prime = true;
        for (auto j = 2; j < i; j++) {
            if (i % j == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            current = i;
            break;
        }
    }
    return *this;
}

int PrimeNumberIterator::operator*() const { return current; }

PrimeNumberRange::PrimeNumberRange(int max) : max{max} {};

PrimeNumberIterator PrimeNumberRange::begin() const { return PrimeNumberIterator{}; }

int PrimeNumberRange::end() const { return max; }
