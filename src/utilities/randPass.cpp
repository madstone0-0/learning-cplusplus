#include <cstdio>
#include <random>

int main() {
    const char characters[]{'A', 'B', 'C', 'D', 'E', 'F', 'G', '1', '2', '3', '4', '5', '6', '7'};
    std::random_device rdEngine{};
    std::uniform_int_distribution<int> intD{0, 13};

    for (size_t i{}; i < 13; i++) {
        printf("%c", characters[intD(rdEngine)]);
    }
    printf("\n");

    return 0;
}
