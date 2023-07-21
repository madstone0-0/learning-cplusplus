// #include <icecream.hpp>
#include "bytes.hpp"

#include <boost/container/flat_map.hpp>
#include <cmath>
#include <iostream>
#include <map>
#include <stdexcept>

#include "utils.hpp"

Bytes::Byte Bytes::convert(const double value, const string& unit, const string& toUnit) {
    using Utils::getIndex, std::exp2, std::abs;
    if (unit == toUnit) return {value, unit};

    const boost::container::flat_map<string, int> unitPowerMap{{"B", 0},   {"KB", 10}, {"MB", 20},
                                                               {"GB", 30}, {"TB", 40}, {"PB", 50}};

    try {
        auto unitPower{unitPowerMap.at(unit)};
        auto toUnitPower{unitPowerMap.at(toUnit)};
        auto multiple = abs(unitPower - toUnitPower);
        auto factor = exp2(multiple);

        double newValue = unitPower > toUnitPower ? value * factor : value / factor;

        return {newValue, toUnit};

    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Invalid unit");
    }
}
