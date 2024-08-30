#pragma once
#include <cstddef>
#include <stdexcept>
namespace Utils {
    template <typename Fn, typename In>
    constexpr bool all(Fn, In*, size_t);

    template <typename T>
    constexpr auto abs(const T& x);

    template <typename T, size_t size>
    size_t getIndex(const T (&list)[size], const T& target);

    template <typename T>
    size_t getIndex(const T* list, const T& target, size_t length);

    template <typename T, typename U>
    auto sliceArray(const T& src, const size_t start, const size_t end);

    template <typename Base, typename T>
    bool instanceof (T * ptr);

    // template <typename T, typename U, size_t length>
    // auto sliceArray(const T* src, const size_t start, const size_t end);

}  // namespace Utils

template <typename Base, typename T>
bool Utils:: instanceof (T * ptr) {
    auto res = dynamic_cast<const Base*>(ptr) != nullptr;
    return res;
}

template <typename Fn, typename In>
constexpr bool Utils::all(Fn function, In* input, size_t length) {
    for (int i{}; i < length; i++) {
        if (!function(input[i])) return false;
    }
    return true;
}

template <typename T>
constexpr auto Utils::abs(const T& x) {
    return x < 0 ? x * -1 : x;
}

template <typename T, size_t size>
size_t Utils::getIndex(const T (&list)[size], const T& target) {
    for (size_t i = 0; i < size; i++) {
        if (list[i] == target) {
            return i;
        }
    }
    throw std::invalid_argument("Target not found in list");
}

template <typename T>
size_t Utils::getIndex(const T* list, const T& target, size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (list[i] == target) {
            return i;
        }
    }
    throw std::invalid_argument("Target not found in list");
}

template <typename T, typename U>
auto Utils::sliceArray(const T& src, const size_t start, const size_t end) {
    if (start >= src.size() || end <= start) throw std::runtime_error("Cannot slice array");
    U dest;
    std::copy(std::begin(src) + start, std::begin(src) + end, std::begin(dest));
    return dest;
}

// template <typename T, typename U, size_t length>
// auto Utils::sliceArray(const T* src, const size_t start, const size_t end) {
//     if (start >= length || end <= start) throw std::runtime_error("Cannot slice array");
//     U dest{};
//     std::copy(std::begin(src) + start, std::begin(src) + end, std::begin(dest));
//     return dest;
// }
