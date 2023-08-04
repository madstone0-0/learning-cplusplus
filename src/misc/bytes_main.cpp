#include <algorithm>
#include <boost/algorithm/string/case_conv.hpp>
#include <iostream>

#include "bytes.hpp"
using std::cout, std::string, boost::to_upper;

int main(int argc, char* argv[]) {
    using namespace Bytes;

    double value{};
    string unit{""};
    string toUnit{""};

    if (argc == 1) {
        cout << "No arguments provided\n";
        return 1;
    }

    if (argc != 4) {
        cout << "Invalid number of arguments\n";
        return 1;
    }

    try {
        value = std::stol(argv[1]);
    } catch (const std::exception& e) {
        printf("Exception: %s\n", e.what());
        return 1;
    }
    unit = argv[2];
    toUnit = argv[3];
    to_upper(unit);
    to_upper(toUnit);

    try {
        Byte result = convert(value, unit, toUnit);
        printf("%g %s = %g %s\n", value, unit.c_str(), result.value, result.unit.c_str());
    } catch (const std::exception& e) {
        printf("Execption: %s\n", e.what());
        return 1;
    }

    return 0;
}
