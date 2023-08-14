#include <array>
#include <cstdio>

int main() {
    using std::array;

    array<int, 4> arr{1, 2, 3, 4};
    printf("The third element is %d.\n", arr.at(2));
    arr.at(2) = 100;
    printf("The third element id %d.\n", arr.at(2));

    unsigned long max = 0;
    array<unsigned long, 5> values{10, 50, 20, 40, 0};
    for (size_t i{0}; i < 5; i++)
        if (values.at(i) > max) max = values.at(i);
    printf("The maximum value is %lu\n", max);

    return 0;
}
