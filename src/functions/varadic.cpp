#include <cstdarg>
#include <cstdint>
#include <cstdio>

/* Varadic Functions */

int sum(size_t n, ...) {
    va_list args;
    va_start(args, n);
    int result{};
    while (n--) {
        auto next = va_arg(args, int);
        result += next;
    }
    va_end(args);
    return result;
}

// int main() {
// auto res = sum(6, 2, 3, 23, 3, 43, 45);
// printf("Sum: %d\n", res);
//     return 0;
// }

/* Varadic Functions */

/* Varadic Template */

// template <typename T>
// constexpr int sum(T x) {
//     return x;
// }
//
// template <typename T, typename... Args>
// constexpr int sum(T x, Args... args) {
//     return x + sum(args...);
// }
//
// int main() {
//     auto res = sum(2, 3, 23, 3, 43, 45);
//     printf("Sum: %d\n", res);
//     return 0;
// }

/* Varadic Template */

/* Fold Expressions */

template <typename... T>
constexpr auto sum(T... args) {
    return (... + args);
}

int main() {
    auto res = sum(2, 3, 23, 3, 43, 45);
    printf("Sum: %d\n", res);
    return 0;
}

/* Fold Expressions */
