#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <map>
#include <stdexcept>
#include <type_traits>

/* Template Examples */
template <typename X, typename Y, typename Z>
struct TemplateClass {
    X foo(Y&);

   private:
    Z* member;
};

template <typename X, typename Y, typename Z>
X template_function(Y& param1, const Z* param2) {}
/* Template Examples */

/* Named Conversion Functions */

// const_cast
void carbon_thaw(const int& carbonated_solo) {
    auto& hibernation_sick_solo = const_cast<int&>(carbonated_solo);
    hibernation_sick_solo++;
}

// static_cast
short increment_as_short(void* target) {
    auto as_short = static_cast<short*>(target);
    // *as_short = *as_short + 1;
    *as_short += 1;
    return *as_short;
}
//
// int main() {
//     short beast{665};
//     auto mark_of_the_beast = increment_as_short(&beast);
//     printf("%d is the mark of the beast\n", mark_of_the_beast);
// }

// reinterpret_cast
// int main() {
//     auto timer = reinterpret_cast<const unsigned long*>(0x1000);
//     printf("Timer is %lu\n", *timer);
// }

// narrow_cast
template <typename To, typename From>
To narrow_cast(From value) {
    const auto converted = static_cast<To>(value);
    const auto backwards = static_cast<From>(converted);
    if (value != backwards) throw std::runtime_error{"Narrowed"};
    return converted;
}

// int main() {
//     int perfect{496};
//     const auto perfect_short = narrow_cast<short>(perfect);
//     printf("Perfect short: %d\n", perfect_short);
//     try {
//         int cyclic{142857};
//         const auto cyclic_short = narrow_cast<short>(cyclic);
//         printf("Cyclic short: %d\n", cyclic_short);
//     } catch (const std::runtime_error& e) {
//         printf("Exception: %s\n", e.what());
//     }
//
//     return 0;
// }

/* Named Conversion Functions */

/* Mean */

template <typename T>
T mean(const T* values, size_t length) {
    T sum{};
    for (size_t i = 0; i < length; i++) {
        sum += values[i];
    }
    return sum / length;
}
//
// int main() {
//     const double doubles[]{1.2, 3.3, 3.34, 34.5};
//     const double dMean = mean<double>(doubles, 4);
//
//     const int ints[]{1, 23, 4, 5, 4, 6, 7, 6, 5, 6};
//     const int iMean = mean<int>(ints, 10);
//
//     const float floats[]{1.2f, 2.32f, 5.34f, 3.23f};
//     const auto fMean = mean(floats, 4);
//
//     printf("%f\n%d\n%f\n", dMean, iMean, fMean);
//
//     return 0;
// }

/* Mean */

/* Type Saftey */

template <typename T>
T square(T x) {
    return x * x;
}
//
// int main() {
//     char myChar{'Q'};
//     auto result = square(&myChar);  // Crashes
//     return 0;
// }

/* Type Saftey */

/* Concepts */

// Type Traits

// constexpr const char* as_str(bool x) { return x ? "True" : "False"; }
//
// int main() {
//     printf("%s\n", as_str(std::is_integral<int>::value));
//     printf("%s\n", as_str(std::is_integral<const int>::value));
//     printf("%s\n", as_str(std::is_integral<char>::value));
//     printf("%s\n", as_str(std::is_integral<uint64_t>::value));
//     printf("%s\n", as_str(std::is_integral<int&>::value));
//     printf("%s\n", as_str(std::is_integral<int*>::value));
//     printf("%s\n", as_str(std::is_integral<float>::value));
//     return 0;
// }

// Requirements
// T, U are types
// requires(T t, U u) {
//     { t == u } -> bool;  // syntactic requirement 1
//     { u == t } -> bool;  // syntactic requirement 2
//     { t != u } -> bool;  // syntactic requirement 3
//     { u != t } -> bool;  // syntactic requirement 4
// }

// Concepts in action
template <typename T>
concept Averageable = requires(T a, T b) {
    std::is_default_constructible<T>::value;
    a += b;
    a / size_t{1};
};

template <Averageable T>
T mean(const T* values, size_t length) {
    T sum{};
    for (size_t i = 0; i < length; i++) {
        sum += values[i];
    }
    return sum / length;
}
//
// int main() {
//     const double doubles[]{1.2, 3.3, 3.34, 34.5};
//     const double dMean = mean<double>(doubles, 4);
//
//     const int ints[]{1, 23, 4, 5, 4, 6, 7, 6, 5, 6};
//     const int iMean = mean<int>(ints, 10);
//
//     const float floats[]{1.2f, 2.32f, 5.34f, 3.23f};
//     const auto fMean = mean(floats, 4);
//
//     printf("%f\n%d\n%f\n", dMean, iMean, fMean);
//
//     auto value1 = 0.0;
//     auto value2 = 1.0;
//     const double* values[]{&value1, &value2};
//     mean(values, 2);  // Compile time error
//
//     return 0;
// }

// Ad Hoc Requires Expressions
template <typename T>
    requires std::is_copy_constructible<T>::value
T get_copy(T* pointer) {
    if (!pointer) throw std::runtime_error{"Null-pointer dereference"};
    return *pointer;
}

struct Highlander {
    Highlander() = default;
    Highlander(const Highlander&) = delete;
};
//
// int main() {
//     auto ptr = new int{69};
//     auto ptr_copy = get_copy(ptr);
//     delete ptr;
//
//     Highlander connor;
//     auto connor_ptr = &connor;
//     auto connor_copy = get_copy(connor_ptr);  // Compile time error
//     return 0;
// }

/* Concepts */

/* static_assert */
// template <typename T>
// T mean(const T* values, size_t length) {
//     static_assert(std::is_default_constructible<T>(),
//                   "Type must be default constructible");
//     static_assert(std::is_copy_constructible<T>(),
//                   "Type must be copy constructible");
//     static_assert(std::is_arithmetic<T>(),
//                   "Type must support addition and subtraction");
//     static_assert(std::is_constructible<T, size_t>(),
//                   "Type must be constructible from size_t.");
//
//     T sum{};
//     for (size_t i = 0; i < length; i++) {
//         sum += values[i];
//     }
//     return sum / length;
// }

/* static_assert */

/* Non Type Template Parameters */
// int& get(int (&arr)[10], size_t index) {
//     if (index >= 10) throw std::out_of_range("Out of bounds");
//     return arr[index];
// }
//
// template <typename T>
// T& get(T (&arr)[10], size_t index) {
//     if (index >= 10) throw std::out_of_range("Out of bounds");
//     return arr[index];
// }
//
// template <typename T, size_t size>
// T& get(T (&arr)[size], size_t index) {
//     if (index >= 10) throw std::out_of_range("Out of bounds");
//     return arr[index];
// }

template <size_t index, typename T, size_t size>
T& get(T (&arr)[size]) {
    static_assert(index < size, "Out-of-bounds access");
    return arr[index];
}

int main() {
    int fib[]{1, 1, 2, 0};
    printf("%d %d %d ", get<0>(fib), get<1>(fib), get<2>(fib));
    get<3>(fib) = get<1>(fib) + get<2>(fib);
    printf("%d\n", get<3>(fib));
    // printf("%d", get<4>(fib));
}

/* Non Type Template Parameters */

/* Varadic Templates */

template <typename T>
struct SimpleUniquePointer {
    SimpleUniquePointer() = default;

    SimpleUniquePointer(T* pointer) : pointer{pointer} {}

    ~SimpleUniquePointer() {
        if (pointer) delete pointer;
    }

    SimpleUniquePointer(const SimpleUniquePointer&) = delete;
    SimpleUniquePointer& operator=(const SimpleUniquePointer&) = delete;

    SimpleUniquePointer(SimpleUniquePointer&& other) noexcept : pointer{other.pointer} { other.pointer = nullptr; }

    SimpleUniquePointer&& operator=(SimpleUniquePointer&& other) noexcept {
        if (pointer) delete pointer;
        pointer = other.pointer;
        other.pointer = nullptr;
        return *this;
    }

   private:
    T* pointer{};
};

template <typename T, typename... Args>
SimpleUniquePointer<T> make_simple_unique(Args... args) {
    return SimpleUniquePointer<T>{new T{args...}};
}

/* Varadic Templates */

/* Excersices */

/* Excersices */
