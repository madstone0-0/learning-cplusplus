#include <bitset>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <limits>
#include <new>
#include <stdexcept>

/* Logical */
// constexpr const char* as_str(bool x) { return x ? "True" : "False"; }
//
// int main() {
//     // Bitwise AND
//     std::cout << std::bitset<4>(0b1100 & 0b1010) << "\n";
//
//     // Bitwise OR
//     std::cout << std::bitset<4>(0b1100 | 0b1010) << "\n";
//
//     // Bitwise XOR
//     std::cout << std::bitset<4>(0b1100 ^ 0b1010) << "\n";
//
//     // Bitwise complement
//     std::cout << std::bitset<4>(~0b1010) << "\n";
//
//     // Bitwise left shift
//     std::cout << std::bitset<8>(0b1010 << 2) << " , "
//               << std::bitset<8>(0b0011 << 4) << "\n";
//
//     // Bitwise right shift
//     std::cout << std::bitset<4>(0b1010 >> 2) << " , "
//               << std::bitset<4>(0b10110011 >> 4) << "\n";
//
//     // AND
//     printf("%s, %s\n", as_str(true && false), as_str(true && true));
//
//     // OR
//     printf("%s, %s\n", as_str(true || false), as_str(false || false));
//
//     // NOT
//     printf("%s, %s\n", as_str(!true), as_str(!true));
//
//     // Tenary
//     printf("%s\n", as_str(1 > 2 ? true : false));
//
//     return 0;
// }
//
/* Logical */

/* Operator Overloading */

// struct CheckedInteger {
//     CheckedInteger(unsigned int value) : value{value} {}
//
//     CheckedInteger operator+(unsigned int other) {
//         CheckedInteger result{value + other};
//         if (result.value < value) throw std::overflow_error("Overflow");
//         return result;
//     }
//
//     const unsigned int value;
// };
//
// int main() {
//     CheckedInteger a{100};
//     auto b = a + 200;
//     printf("a + 200: %u\n", b.value);
//     try {
//         auto c = a + std::numeric_limits<unsigned int>::max();
//     } catch (const std::overflow_error& e) {
//         printf("(a + max) Exception: %s\n", e.what());
//     }
//
//     return 0;
// }

/* Operator Overloading */

/* Taking Control of the Free Store */

// Heaps and Buckets

// struct Bucket {
//     const static size_t data_size{4096};
//     std::byte data[data_size];
// };
//
// struct Heap {
//     void* allocate(size_t bytes) {
//         if (bytes > Bucket::data_size) throw std::bad_alloc{};
//         for (size_t i{}; i < n_heap_buckets; i++) {
//             if (!bucket_used[i]) {
//                 bucket_used[i] = true;
//                 return &buckets[i].data;
//             }
//         }
//         throw std::bad_alloc{};
//     }
//
//     void free(void* p) {
//         for (size_t i{}; i < n_heap_buckets; i++) {
//             if (buckets[i].data == p) {
//                 bucket_used[i] = false;
//                 return;
//             }
//         }
//     }
//
//     static const size_t n_heap_buckets{10};
//     Bucket buckets[n_heap_buckets]{};
//     bool bucket_used[n_heap_buckets]{};
// };
//
// Heap heap;
//
// void* operator new(size_t n_bytes) { return heap.allocate(n_bytes); }
//
// void operator delete(void* p) { return heap.free(p); }
//
// int main() {
//     printf("Buckets:   %p\n", heap.buckets);
//
//     auto breakfast = new unsigned int{0xC0FFEE};
//     auto dinner = new unsigned int{0xDEADBEEF};
//
//     printf("Breakfast: %p 0x%x\n", breakfast, *breakfast);
//     printf("Dinner:    %p 0x%x\n", dinner, *dinner);
//
//     delete breakfast;
//     delete dinner;
//
//     try {
//         while (true) {
//             new char;
//             printf("Allocated a char.\n");
//         }
//     } catch (const std::bad_alloc&) {
//         printf("std::bad_alloc caught.\n");
//     }
//
//     return 0;
// }

// Placement Operators

struct Point {
    Point() : x{}, y{}, z{} { printf("Point at %p constructed\n", this); }

    ~Point() { printf("Point at %p destructed\n", this); }

    double x, y, z;
};

int main() {
    const auto point_size = sizeof(Point);
    std::byte data[3 * point_size];

    printf("Data starts at %p\n", data);
    auto point1 = new (&data[0 * point_size]) Point{};
    auto point2 = new (&data[1 * point_size]) Point{};
    auto point3 = new (&data[2 * point_size]) Point{};

    point1->~Point();
    point2->~Point();
    point3->~Point();

    return 0;
}

/* Taking Control of the Free Store */
