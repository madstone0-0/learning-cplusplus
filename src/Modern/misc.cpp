#include <iostream>

using namespace std;

template <typename Type, Type val>
constexpr Type constant = val;

constexpr auto const IntConst42 = constant<int, 42>;

template <auto val>
constexpr auto constantAuto = val;

constexpr auto const IntConst42AutoTemplate = constantAuto<42>;

int main() {
    cout << IntConst42 << '\n';
    cout << IntConst42AutoTemplate << '\n';
    return 0;
}
