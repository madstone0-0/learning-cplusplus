// #include <icecream.hpp>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

#include "utils.cpp"

namespace Bytes {
    using std::string;

    struct Byte {
        double value;
        string unit;
    };

    Byte convert(const double value, const string unit, const string toUnit) {
        using Utils::getIndex, std::pow, std::abs;
        if (unit == toUnit) return {value, unit};

        string units[]{"B", "KB", "MB", "GB", "TB", "PB"};
        int powers[]{0, 10, 20, 30, 40, 50};

        try {
            auto unitPower{powers[getIndex(units, unit)]};
            auto toUnitPower{powers[getIndex(units, toUnit)]};
            auto multiple = abs(unitPower - toUnitPower);

            double newValue = unitPower > toUnitPower ? value * pow(2, multiple) : value / pow(2, multiple);

            return {newValue, toUnit};

        } catch (const std::invalid_argument& e) {
            throw std::invalid_argument("Invalid unit");
        }
    }

}  // namespace Bytes
