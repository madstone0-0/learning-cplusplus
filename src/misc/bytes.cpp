#include <algorithm>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>

using std::cout, std::string;

struct Byte {
    double value;
    string unit;
};

template <typename T>
constexpr T abs(T x) {
    return x > 0 ? x : x * -1;
}

constexpr unsigned long long exp(unsigned long long x, int pow) {
    if (pow == 0) return 1;
    auto res{1};
    for (int i = 0; i < pow; i++) {
        res *= x;
    }
    return res;
}

template <typename T, size_t size>
constexpr size_t getIndex(const T (&list)[size], const T& target) {
    for (size_t i = 0; i < size; i++) {
        if (list[i] == target) {
            return i;
        }
    }
    throw std::invalid_argument("Target not found in list");
}

Byte convert(const double value, const string unit, const string toUnit) {
    if (unit == toUnit) return {value, unit};

    string units[]{"B", "KB", "MB", "GB", "TB", "PB"};
    int powers[]{0, 10, 20, 30, 40, 50};

    try {
        auto unitPower{powers[getIndex(units, unit)]};
        auto toUnitPower{powers[getIndex(units, toUnit)]};
        auto multiple = abs(unitPower - toUnitPower);
        double newValue = unitPower > toUnitPower ? value * exp(2, multiple) : value / exp(2, multiple);

        return {newValue, toUnit};

    } catch (const std::exception& e) {
        throw std::invalid_argument("Invalid unit");
    }
}

int main(int argc, char* argv[]) {
    double value{};
    string unit{""};
    string toUnit{""};

    if (argc == 1) {
        cout << "No arguments provided\n";
        return 1;
    } else if (argc != 4) {
        cout << "Invalid number of arguments\n";
        return 1;
    } else {
        try {
            value = std::stol(argv[1]);

        } catch (const std::exception& e) {
            printf("Exception: %s\n", e.what());
            return 1;
        }
        unit = argv[2];
        toUnit = argv[3];
        std::transform(unit.begin(), unit.end(), unit.begin(), ::toupper);
        std::transform(toUnit.begin(), toUnit.end(), toUnit.begin(), ::toupper);
    }

    try {
        Byte result = convert(value, unit, toUnit);
        printf("%g %s = %g %s\n", value, unit.c_str(), result.value, result.unit.c_str());
    } catch (const std::exception& e) {
        printf("Execption: %s\n", e.what());
        return 1;
    }

    return 0;
}
