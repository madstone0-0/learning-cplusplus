// #include <icecream.hpp>
#include "bytes.hpp"

#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "utils.hpp"

Bytes::Byte Bytes::convert(const double value, const string& unit, const string& toUnit) {
    using Utils::getIndex, std::exp2, std::abs;
    if (unit == toUnit) return {value, unit};

    // string units[]{"B", "KB", "MB", "GB", "TB", "PB"};
    // int powers[]{0, 10, 20, 30, 40, 50};

    const std::array<string, 6> units{"B", "KB", "MB", "GB", "TB", "PB"};
    const std::array<int, 6> powers{0, 10, 20, 30, 40, 50};

    try {
        auto unitPower{powers[getIndex(units.data(), unit, units.size())]};
        auto toUnitPower{powers[getIndex(units.data(), toUnit, units.size())]};
        auto multiple = abs(unitPower - toUnitPower);
        auto factor = exp2(multiple);

        double newValue = unitPower > toUnitPower ? value * factor : value / factor;

        return {newValue, toUnit};

    } catch (const std::invalid_argument& e) {
        throw std::invalid_argument("Invalid unit");
    }
}
