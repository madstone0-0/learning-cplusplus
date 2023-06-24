#include <cmath>
#include <cstdlib>

namespace Algorithms {

    template <typename T, size_t n>
    void bubble(T (&arr)[n]);

    template <typename T, size_t n>
    void insertion(T (&arr)[n]);

    template <typename T, size_t n>
    void merge(T (&arr)[n], size_t start, size_t end);

    template <typename T, size_t n>
    void shell(T (&arr)[n]);
}  // namespace Algorithms
