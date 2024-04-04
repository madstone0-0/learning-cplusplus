#include <iostream>

/*
Creates a copy of a function where ever
it is called
*/
constexpr inline auto square(int num) { return num * num; }

int main() {
    std::cout << "Inline" << std::endl;
    std::cout << "Square of 5 is " << square(5) << std::endl;
    return 0;
}
