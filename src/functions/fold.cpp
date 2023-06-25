#include <cstdio>

template <typename Fn, typename In, typename Out>
constexpr Out fold(Fn function, In* input, size_t length, Out initial) {
    Out result{function(initial, input[0])};
    for (size_t i{1}; i < length; i++) {
        result = function(result, input[i]);
    }
    return result;
}

int main() {
    int data[]{100, 200, 300, 400, 500};
    size_t data_len = 5;
    auto sum = fold([](auto x, auto y) { return x + y; }, data, data_len, 0);
    printf("Sum: %d\n", sum);

    auto max = fold([](auto x, auto y) { return x > y ? x : y; }, data, data_len, 0);
    printf("Max: %d\n", max);

    auto min = fold([](auto x, auto y) { return x < y ? x : y; }, data, data_len, data[0]);
    printf("Min: %d\n", min);

    int index{};
    auto greaterThan200 = fold(
        [&](auto x, auto y) {
            if (x > 200) index++;
            if (y > 200) index++;
            return index;
        },
        data, data_len, data[0]);
    printf("Greater than 200: %d\n", greaterThan200);

    return 0;
}
