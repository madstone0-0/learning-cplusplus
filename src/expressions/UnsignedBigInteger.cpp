#include <algorithm>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>

template <typename T>
concept Integer = requires(T a, T b) { a | b; };

template <Integer T>
struct UnsignedBigInteger {
    UnsignedBigInteger() = default;
    UnsignedBigInteger(T integer) {
        std::copy(static_cast<const uint8_t*>(static_cast<const void*>(&integer)),
                  static_cast<const uint8_t*>(static_cast<const void*>(&integer)) + sizeof(T), bytes);
    }

    explicit operator int() const {
        int converted;
        std::memcpy(&converted, bytes, sizeof(int));
        const auto backwards = static_cast<T>(converted);
        if (converted != backwards) std::runtime_error("Narrowed!");
        return converted;
    }

    // UnsignedBigInteger(T integer) : bytes{*reinterpret_cast<uint8_t*>(&integer)} {}
    UnsignedBigInteger operator+(UnsignedBigInteger other) {
        T value1;
        T value2;
        std::memcpy(&value1, bytes, sizeof(T));
        std::memcpy(&value2, other.bytes, sizeof(T));
        auto sum = value1 + value2;
        if (sum < value1) throw std::overflow_error("Overflow!");
        UnsignedBigInteger res{sum};
        return res;
    }
    UnsignedBigInteger operator+(T other) {
        T value;
        std::memcpy(&value, bytes, sizeof(T));
        auto sum = value + other;
        if (sum < value) throw std::overflow_error("Overflow!");
        UnsignedBigInteger res{sum};
        return res;
    }

    UnsignedBigInteger operator-(T other) {
        T value;
        std::memcpy(&value, bytes, sizeof(T));
        auto diff = value - other;
        if (diff > value) throw std::overflow_error("Overflow!");
        UnsignedBigInteger res{diff};
        return res;
    }

    UnsignedBigInteger operator-(UnsignedBigInteger other) {
        T value1;
        T value2;
        std::memcpy(&value1, bytes, sizeof(T));
        std::memcpy(&value2, other.bytes, sizeof(T));
        auto diff = value1 - value2;
        if (diff > value1) throw std::overflow_error("Overflow!");
        UnsignedBigInteger res{diff};
        return res;
    }

    uint8_t bytes[sizeof(T)];
};

int main() {
    UnsignedBigInteger x{909090909090};
    UnsignedBigInteger y{909090909090};
    UnsignedBigInteger u{9};
    UnsignedBigInteger a = 0x123456789abcdef0;
    UnsignedBigInteger b = 0xfedcba9876543210;
    int z{9};
    long i{909090909090};
    // auto res = x + y;
    try {
        auto res2 = x + 9999999999999999999;

    } catch (std::exception& e) {
        printf("Execption: %s\n", e.what());
    }
    auto res3 = x + i;
    auto res4 = u + 1;

    try {
        std::cout << "res3: " << static_cast<int>(res3) << "\n";
    } catch (std::runtime_error& e) {
        printf("Runtime: %s", e.what());
    }

    std::cout << "res4: " << static_cast<int>(res4) << "\n";

    return 0;
}
