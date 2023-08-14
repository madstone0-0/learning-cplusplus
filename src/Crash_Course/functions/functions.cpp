#include <cstdint>
#include <cstdio>
#include <functional>

/* Function Pointers */

float add(float a, int b) { return a + b; }

float sub(float a, int b) { return a - b; }

using operation_func = float (*)(float, int);

// int main() {
//     const float x{100};
//     const int y{100};
//
//     // float (*operation)(float, int){};
//
//     operation_func operation{};
//     printf("operation initialized to %p\n", operation);
//
//     operation = &add;
//     printf("&add = %p\n", operation);
//     printf("%g + %d = %g\n", x, y, operation(x, y));
//
//     operation = sub;
//     printf("&sub = %p\n", operation);
//     printf("%g - %d = %g\n", x, y, operation(x, y));
//
//     return 0;
// }

/* Function Pointers */

/* Function Call operator */

struct CountIf {
    CountIf(char x) : x{x} {}
    size_t operator()(const char *str) const {
        size_t index{}, result{};
        while (str[index]) {
            if (str[index] == x) result++;
            index++;
        }
        return result;
    }

   private:
    const char x;
};

// int main() {
//     CountIf s_counter{'s'};
//     auto sally = s_counter("Sally sells seashells by the seashore.");
//     printf("Sally: %zu\n", sally);
//
//     auto sailor = s_counter("Sailor went to sea to see what he could see.");
//     printf("Sailor: %zu\n", sailor);
//
//     auto buffalo = CountIf{'f'}(
//         "Buffalo buffalo Buffalo buffalo "
//         "buffalo buffalo Buffalo buffalo.");
//     printf("Buffalo: %zu\n", buffalo);
//
//     return 0;
// }

/* Function Call operator */

/* Lambda Expressions */

// template <typename Fn>
// void transform(Fn fn, const int* in, int* out, size_t length) {
//     for (size_t i{}; i < length; i++) {
//         out[i] = fn(in[i]);
//     }
// }

// int main() {
//     const size_t len{3};
//     int base[]{1, 2, 3}, a[len]{}, b[len]{}, c[len]{}, d[len]{};
//
//     transform([](int x) { return 1; }, base, a, len);
//     transform([](int x) { return x; }, base, b, len);
//     transform([](int x) { return 10 * x + 5; }, base, c, len);
//     transform([](int x) { return 2 * x; }, base, d, len);
//
//     for (size_t i = 0; i < len; i++) {
//         printf("Element %zu: %d %d %d %d\n", i, a[i], b[i], c[i], d[i]);
//     }
//
//     return 0;
// }

// Default Args

// int main() {
//     auto increment = [](auto x, int y = 1) { return x + y; };
//     printf("increment(10): %d\n", increment(10));
//     printf("increment(10, 5): %d\n", increment(10, 5));
//     printf("increment(10.2, 5): %g\n", increment(10.2, 5));
//     return 0;
// }

// Generic Lambda

template <typename Fn, typename T>
void transform(Fn fn, const T *in, T *out, size_t n) {
    for (size_t i = 0; i < n; i++) {
        out[i] = fn(in[i]);
    }
}

// int main() {
//     constexpr size_t len{3};
//     int baseInt[]{1, 2, 3}, a[len];
//     float baseFloat[]{10.f, 20.f, 30.f}, b[len];
//     auto translate = [](auto x) -> decltype(10 * x * 5) { return 10 * x + 5; };
//
//     transform(translate, baseInt, a, len);
//     transform(translate, baseFloat, b, len);
//
//     for (size_t i = 0; i < len; i++) {
//         printf("Element %zu: %d %f\n", i, a[i], b[i]);
//     }
//
//     return 0;
// }

/* Lambda Captures */
// int main() {
//     char to_count{'s'};
//     size_t tally{};
//     // auto s_counter = [to_count, &tally](const char* string) {
//     //     size_t index{}, result{};
//     //     while (string[index]) {
//     //         if (string[index] == to_count) result++;
//     //         index++;
//     //     }
//     //     tally += result;
//     //     return result;
//     // };
//
//     // Default Capture
//     // By reference
//     // auto s_counter = [&](const char* string) {
//     //     size_t index{}, result{};
//     //     while (string[index]) {
//     //         if (string[index] == to_count) result++;
//     //         index++;
//     //     }
//     //     tally += result;
//     //     return result;
//     // };
//
//     // By value
//     // auto s_counter = [=](const char* string) mutable {
//     //     size_t index{}, result{};
//     //     while (string[index]) {
//     //         if (string[index] == to_count) result++;
//     //         index++;
//     //     }
//     //     tally += result; // Tally is copied so it doesn't get updated in the function scope
//     //     return result;
//     // };
//
//     // Mixing
//     auto s_counter = [&, to_count](const char* string) {
//         size_t index{}, result{};
//         while (string[index]) {
//             if (string[index] == to_count) result++;
//             index++;
//         }
//         tally += result;
//         return result;
//     };
//
//     printf("Tally: %zu\n", tally);
//     auto sally = s_counter("Sally sells seashells by the seashore.");
//     printf("Sally: %zu\n", sally);
//     printf("Tally: %zu\n", tally);
//     auto sailor = s_counter("Sailor went to sea to see what he could see.");
//     printf("Sailor: %zu\n", sailor);
//     printf("Tally: %zu\n", tally);
//
//     return 0;
// }

// Initializer expressions in capture lists

// int main() {
//     char to_count{'s'};
//     size_t tally{};
//
//     auto s_counter = [&tally, count = to_count](const char* string) {
//         size_t index{}, result{};
//         while (string[index]) {
//             if (string[index] == count) result++;
//             index++;
//         }
//         tally += result;
//         return result;
//     };
//
//     return 0;
// }

// Capturing this

// struct LambdaFactory {
//     LambdaFactory(char in) : to_count{in}, tally{} {}
//     auto make_lambda() {
//         return [this](const char* string) {
//             size_t index{}, result{};
//             while (string[index]) {
//                 if (string[index] == to_count) result++;
//                 index++;
//             }
//             tally += result;
//             return result;
//         };
//     }
//
//     const char to_count;
//     size_t tally;
// };
//
// int main() {
//     LambdaFactory factory{'s'};
//     auto counter = factory.make_lambda();
//
//     printf("Tally: %zu\n", factory.tally);
//     printf("Sally: %zu\n", counter("Sally sells seashells by the seashore."));
//     printf("Tally: %zu\n", factory.tally);
//     printf("Sailor: %zu\n", counter("Sailor went to sea to see what he could see."));
//     printf("Tally: %zu\n", factory.tally);
// }

/* Lambda Captures */

// Constexpr Lambda Expressions

struct LambdaFactory {
    LambdaFactory(char in) : to_count{in}, tally{} {}
    auto make_lambda() {
        return [this](const char *string) constexpr {
            size_t index{}, result{};
            while (string[index]) {
                if (string[index] == to_count) result++;
                index++;
            }
            tally += result;
            return result;
        };
    }

    const char to_count;
    size_t tally;
};

/* Lambda Expressions */

/* std::function */

void static_function() { printf("A static function.\n"); }

size_t countSpaces(const char *string) {
    size_t index{}, result{};
    while (string[index]) {
        if (string[index] == ' ') result++;
        index++;
    }
    return result;
}

int main() {
    std::function<void()> fn{[]() { printf("A Lambda\n"); }};

    try {
        fn();
    } catch (const std::bad_function_call &e) {
        printf("Exception: %s\n", e.what());
    }

    fn = static_function;
    fn();

    std::function<size_t(const char *)> fns[]{countSpaces, CountIf{'e'}, [](const char *string) {
                                                  size_t index{};
                                                  while (string[index]) index++;
                                                  return index;
                                              }};
    auto text = "Sailor went to sea to see what he could see.";
    size_t index{};
    for (const auto &fn : fns) {
        printf("fn #%zu %zu\n", index++, fn(text));
    }

    return 0;
}

/* std::function */
