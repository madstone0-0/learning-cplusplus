#include <cstddef>
#include <stdexcept>
namespace Utils {
    template <typename Fn, typename In>
    constexpr bool all(Fn, In*, size_t);

    template <typename T, size_t size>
    constexpr size_t getIndex(const T (&list)[size], const T& target);

}  // namespace Utils

template <typename Fn, typename In>
constexpr bool Utils::all(Fn function, In* input, size_t length) {
    for (int i{}; i < length; i++) {
        if (!function(input[i])) return false;
    }
    return true;
}

template <typename T, size_t size>
constexpr size_t Utils::getIndex(const T (&list)[size], const T& target) {
    for (size_t i = 0; i < size; i++) {
        if (list[i] == target) {
            return i;
        }
    }
    throw std::invalid_argument("Target not found in list");
}
