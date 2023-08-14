#include <algorithm>
#include <cctype>
#include <concepts>
#include <cstdio>
#include <type_traits>

template <typename T>
concept Integer = requires(T a, T b) {
                      std::is_integral<T>::value;
                      a | b;
                  };

template <Integer T>
T mode(const T* values, size_t length) {
    if (values == nullptr || length == 0) return 0;
    T mode = 0;
    int max_appear = 0;
    int curr_appear = 0;

    for (size_t i = 0; i < length; i++) {
        for (size_t j = 0; j < length; j++) {
            if (values[i] == values[j]) {
                curr_appear++;
            }
        }

        if (curr_appear > max_appear) {
            mode = values[i];
            max_appear = curr_appear;
        }

        curr_appear = 0;
    }

    return mode;
}

int main() {
    int values1[]{1, 2, 3, 3, 3, 3, 5, 6};
    float values2[]{1.2f, 3.4f, 2.3f, 2.3f, 2.3f, 3.4f};

    // int values[]{};
    printf("Mode: %d\n", mode(values1, 8));
    // printf("Mode: %f\n", mode(values2, 6));
}
