#pragma once
#include <string>
#include <string_view>

namespace Bytes {
    using std::string, std::string_view;
    struct Byte {
        double value;
        string unit;
    };

    Byte convert(const double, const string&, const string&);
}  // namespace Bytes
