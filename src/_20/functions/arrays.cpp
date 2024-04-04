#include <fmt/format.h>

#include <array>
#include <iostream>

int main() {
    auto display = [](const auto& c) {
        for (const auto& i : c) {
            // std::cout << fmt::format("{} ", i);
            std::cout << i << " ";
        }
        std::cout << '\n';
    };

    int cArray[]{1, 2, 3, 4, 5, 7, 8, 9};

    display(std::to_array(cArray));
}
