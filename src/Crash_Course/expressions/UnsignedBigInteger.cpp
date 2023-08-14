#include <algorithm>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <type_traits>

template <typename T>
concept Integer = requires(T a, T b) { a | b; };

struct Bytes {
    unsigned char* bytes;
    int sizeofType;
};

// template <Integer T>
struct UnsignedBigInteger {
    UnsignedBigInteger() = default;
    UnsignedBigInteger(unsigned long long integer) : starting_size{8}, size{8}, bytes{new uint8_t[size]} {
        for (auto i = 0; i < size; i++) {
            bytes[i] = static_cast<uint8_t>(integer >> (i * 8));
        }
        trim();
    }

    explicit operator int() const {
        int converted;
        unsigned long long original;
        std::memcpy(&converted, bytes, sizeof(int));
        std::memcpy(&original, bytes, starting_size);
        // const auto backwards = static_cast<unsigned long long>(converted);
        if (converted != original) throw std::runtime_error("Narrowed!\n");
        return converted;
    }

    // UnsignedBigInteger(T integer) : bytes{*reinterpret_cast<uint8_t*>(&integer)} {}
    UnsignedBigInteger operator+(const UnsignedBigInteger& other) const {
        unsigned long long value1;
        unsigned long long value2;
        std::memcpy(&value1, bytes, starting_size);
        std::memcpy(&value2, other.bytes, other.starting_size);
        auto sum = value1 + value2;
        if (sum < value1) throw std::overflow_error("Overflow!");
        UnsignedBigInteger res{sum};
        return res;
    }

    ~UnsignedBigInteger() { delete[] bytes; }

    UnsignedBigInteger operator+(const unsigned long long other) const {
        unsigned long long value;
        std::memcpy(&value, bytes, starting_size);
        auto sum = value + other;
        if (sum < value) throw std::overflow_error("Overflow!");
        UnsignedBigInteger res{sum};
        return res;
    }

    // friend std::ostream& operator<<(std::ostream& os, const UnsignedBigInteger& num) {
    //     for (int i = static_cast<int>(num.size) - 1; i >= 0; i--) {
    //         os << (static_cast<int>(num.bytes[i]) & 0xff);
    //     }
    //     return os;
    // }

    UnsignedBigInteger operator-(unsigned long long other) {
        unsigned long long value;
        std::memcpy(&value, bytes, starting_size);
        auto diff = value - other;
        if (diff > value) throw std::overflow_error("Overflow!");
        UnsignedBigInteger res{diff};
        return res;
    }

    UnsignedBigInteger operator-(const UnsignedBigInteger& other) const {
        unsigned long long value1;
        unsigned long long value2;
        std::memcpy(&value1, bytes, starting_size);
        std::memcpy(&value2, other.bytes, starting_size);
        auto diff = value1 - value2;
        if (diff > value1) throw std::overflow_error("Overflow!");
        UnsignedBigInteger res{diff};
        return res;
    }

   private:
    size_t starting_size;
    size_t size;
    uint8_t* bytes;

    void trim() {
        while (size > 1 && bytes[size - 1] == 0) {
            size--;
        }
    }
};

int main() {
    UnsignedBigInteger x{909090909090};
    UnsignedBigInteger y{909090909090};
    UnsignedBigInteger u{9};
    int z{9};
    long i{909090909090};
    auto res = x + y;
    try {
        auto res2 = x + std::numeric_limits<unsigned long long>::max();

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

    std::cout << "res4: " << static_cast<int>(u + 9) << "\n";

    return 0;
}
