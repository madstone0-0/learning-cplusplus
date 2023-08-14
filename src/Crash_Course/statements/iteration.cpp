#include <cstdint>
#include <cstdio>
#include <limits>

/* While loops */

template <typename T>
bool doubleReturnOverflow(T& x) {
    const auto original = x;
    x *= 2;
    return original > x;
}

// int main() {
//     uint8_t x{1};
//     uint16_t y{1};
//     uint32_t z{1};
//
//     printf("uint8_t:\n===\n");
//     while (!doubleReturnOverflow(x)) {
//         printf("%u ", x);
//     }
//
//     printf("\n\nuint16_t:\n===\n");
//     while (!doubleReturnOverflow(y)) {
//         printf("%u ", y);
//     }
//
//     printf("\n\nuint32_t:\n===\n");
//     while (!doubleReturnOverflow(z)) {
//         printf("%u ", z);
//     }
//
//     return 0;
// }
/* While loops */

/* Do-While loops */

// int main() {
//     uint8_t x{1};
//     uint16_t y{1};
//     uint32_t z{1};
//
//     printf("uint8_t:\n===\n");
//     do {
//         printf("%u ", x);
//     } while (!doubleReturnOverflow(x));
//
//     printf("\n\nuint16_t:\n===\n");
//     do {
//         printf("%u ", y);
//     } while (!doubleReturnOverflow(y));
//
//     printf("\n\nuint32_t:\n===\n");
//     do {
//         printf("%u ", z);
//     } while (!doubleReturnOverflow(z));
//
//     return 0;
// }

/* Do-While loops */

/* For loops */

// Iterating with an index
// int main() {
//     const int x[]{1, 1, 2, 3, 5, 8};
//     printf("i: x[i]\n");
//     for (int i{}; i < 6; i++) {
//         printf("%d: %d\n", i, x[i]);
//     }
// }

// Range based for loops
// int main() {
//     const int x[]{1, 1, 2, 3, 5, 8};
//     for (const auto element : x) {
//         printf("%d ", element);
//     }
// }

// Iterators
struct FibonacciIterator {
    bool operator!=(int x) const { return x >= current; }

    FibonacciIterator& operator++() {
        const auto temp = current;
        current += last;
        last = temp;
        return *this;
    }

    int operator*() const { return current; }

   private:
    int current{1};
    int last{1};
};

struct FibonacciRange {
    explicit FibonacciRange(int max) : max{max} {}
    FibonacciIterator begin() const { return FibonacciIterator{}; }

    int end() const { return max; }

   private:
    const int max;
};

int main() {
    for (const auto i : FibonacciRange{5000}) {
        printf("%d ", i);
    }
    return 0;
}

/* For loops */
