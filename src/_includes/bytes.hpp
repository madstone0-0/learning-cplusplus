#ifndef BYTES_INCLUDE
#define BYTES_INCLUDE
#include <string>

namespace Bytes {
    using std::string;
    struct Byte {
        double value;
        string unit;
    };

    Byte convert(const double, const string&, const string&);
}  // namespace Bytes
#endif
