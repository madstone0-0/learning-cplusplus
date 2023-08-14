#include <bitset>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <limits>
#include <new>
#include <stdexcept>
/* Type Conversions */

/* Implicit Type Conversions */

// Pointer to void*
void print_addr(void* x) { printf("%p\n", x); }

// int main() {
//     // Narrowing Conversion
//     auto x1 = 2.7182818284590452353602874713527L;
//     uint8_t y1 = x1;
//     // uint8_t y = {x};
//
//     // Integer to Integer Conversion
//     // 0b111111111 is 511
//     uint8_t x2 = 0b111111111;  // Converts to 255
//     int8_t y2 = 0b111111111;   // Converts to 511
//     printf("x: %u\ny: %d\n", x2, y2);
//
//     printf("\n");
//
//     // Floating point to Floating Point Conversion
//     float x3 = std::numeric_limits<float>::max();
//     long double y3 = std::numeric_limits<double>::max();
//     float z = std::numeric_limits<long double>::max();  // Undefined Behavior
//     printf("x: %g\ny: %Lg\nz: %g\n", x3, y3, z);
//
//     printf("\n");
//
//     // Pointer to void*
//
//     int x4{};
//     print_addr(&x4);
//     print_addr(nullptr);
//
//     return 0;
// }

/* Implicit Type Conversions */

/* Explicit Type Conversions */

// C-style cast
void trainwreck(const char* read_only) {
    // auto as_unsigned = (unsigned char*)read_only;
    auto as_unsigned = reinterpret_cast<unsigned char*>(const_cast<char*>(read_only));
    *as_unsigned = 'b';
}

// int main() {
//     // Braced explicit conversion
//     int32_t a = 100;
//     int64_t b{a};
//     if (a == b) printf("Non-narrowing type conversion\n");
//     // int32_t c{b}; // Narrowing
//
//     // C-style cast
//     char erza[] = "Erza";
//     printf("Before: %s\n", erza);
//     trainwreck(erza);
//     printf("After: %s\n", erza);
//
//     return 0;
// }

/* Explicit Type Conversions */

/* User-Defined Type Conversions */

struct ReadOnlyInt {
    ReadOnlyInt(int val) : val{val} {}
    // ReadOnlyInt to int conversion
    explicit operator int() const { return val; }

   private:
    const int val;
};

int main() {
    ReadOnlyInt ans{42};
    auto ten_ans = static_cast<int>(ans) * 10;
    printf("Ten times the answer is %d\n", ten_ans);
    return 0;
}

/* User-Defined Type Conversions */

/* Type Conversions */
