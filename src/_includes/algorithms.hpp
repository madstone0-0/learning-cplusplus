#ifndef ALGORITHMS_INCLUDE
#define ALGORITHMS_INCLUDE

#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <cstdlib>
#include <iterator>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include "concepts.hpp"
#include "utils.hpp"

namespace algorithms {

    using std::next, std::advance, std::distance, std::prev;
    using namespace mts::concepts;
    namespace sorting {

        template <typename T, size_t n, typename Compare>
        size_t bubble(T (&arr)[n], Compare compare);

        template <Traversable Itr, typename Compare = std::less<>>
        size_t bubble(Itr begin, Itr end, Compare compare);

        template <typename T, size_t n, typename Compare>
        size_t insertion(T (&arr)[n], Compare compare);

        template <Traversable Itr, typename Compare = std::less<>>
        size_t insertion(Itr begin, Itr end, Compare compare);

        template <typename T, size_t n, typename Compare>
        size_t merge(T (&arr)[n], size_t start, size_t end, Compare compare);

        template <Traversable Itr, typename Compare = std::less<>>
        size_t merge(Itr begin, Itr end, Compare compare);

        template <typename T, size_t n, typename Compare>
        size_t shell(T (&arr)[n], Compare compare);

        template <Traversable Itr, typename Compare = std::less<>>
        size_t shell(Itr begin, Itr end, Compare compare);

        template <typename T, size_t n, typename Compare>
        size_t quicksort(T (&arr)[n], size_t low, size_t high, Compare compare);

        template <Traversable Itr, typename Compare = std::less<>>
        size_t quicksort(Itr begin, Itr end, Compare compare);

    }  // namespace sorting

    namespace searching {
        template <typename T, size_t n>
        std::pair<std::optional<size_t>, size_t> binary(const T (&arr)[n], const T& target);

        template <Traversable Itr>
        std::pair<Itr, size_t> binary(Itr begin, Itr end, const std::iter_value_t<Itr>& target);

    }  // namespace searching

}  // namespace algorithms
#endif

using namespace mts::concepts;

template <typename T, size_t n, typename Compare>
size_t algorithms::sorting::bubble(T (&arr)[n], Compare compare) {
    size_t numOfOpertations{};
    size_t length = n - 1;
    for (int i = length; i >= 0; i--) {
        for (auto j = 0; j < i; j++) {
            if (compare(arr[j + 1], arr[j])) {
                std::swap(arr[j], arr[j + 1]);
                numOfOpertations++;
            }
        }
    }
    return numOfOpertations;
}

template <Traversable Itr, typename Compare>
size_t algorithms::sorting::bubble(Itr begin, Itr end, Compare compare) {
    size_t numOfOpertations{};
    for (auto i = begin; i != end; i++) {
        for (auto j = i + 1; j != end; j++) {
            if (compare(*j, *i)) {
                std::iter_swap(j, i);
                numOfOpertations++;
            }
        }
    }
    return numOfOpertations++;
}

template <typename T, size_t n, typename Compare>
size_t algorithms::sorting::insertion(T (&arr)[n], Compare compare) {
    size_t numOfOpertations{};
    for (auto i = 1; i < n; i++) {
        int j = i - 1;
        T element_next = arr[i];
        while (compare(element_next, arr[j]) && j >= 0) {
            arr[j + 1] = arr[j];
            j--;
            numOfOpertations++;
        }
        arr[j + 1] = element_next;
        numOfOpertations++;
    }
    return numOfOpertations;
}

template <Traversable Itr, typename Compare>
size_t algorithms::sorting::insertion(Itr begin, Itr end, Compare compare) {
    size_t numOfOpertations{};
    // for (auto i = next(begin); i != end; i++) {
    //     auto elementNext = *i;
    //     auto j = prev(i);
    //     while (compare(elementNext, *j) && j != begin) {
    //         *(next(j)) = *j;
    //         advance(j, -1);
    //         numOfOpertations++;
    //         // if (j != begin) {
    //         //     j--;
    //         //     numOfOpertations++;
    //         // } else
    //         //     break;
    //     }
    //     *(next(j)) = elementNext;
    //     numOfOpertations++;
    // }

    for (auto i = begin; i != end; i++) {
        std::rotate(std::upper_bound(begin, i, *i), i, i + 1);
        numOfOpertations++;
    }

    return numOfOpertations;
}

template <typename T, size_t n, typename Compare>
size_t algorithms::sorting::merge(T (&arr)[n], size_t start, size_t end, Compare compare) {
    size_t numOfOpertations{0};
    if (start < end) {
        size_t mid = start + (end - start) / 2;
        merge(arr, start, mid, compare);
        merge(arr, mid + 1, end, compare);

        numOfOpertations = 0;

        // Merge
        size_t len_left = mid - start + 1;
        size_t len_right = end - mid;

        std::unique_ptr<T[]> left(new T[len_left]);
        std::unique_ptr<T[]> right(new T[len_right]);

        for (size_t i = 0; i < len_left; i++) {
            left[i] = arr[start + i];
            numOfOpertations++;
        }

        for (size_t i = 0; i < len_right; i++) {
            right[i] = arr[mid + 1 + i];
            numOfOpertations++;
        }

        size_t a = 0;
        size_t b = 0;
        size_t c = start;

        while (a < len_left && b < len_right) {
            if (compare(left[a], right[b])) {
                arr[c] = left[a];
                a++;
                numOfOpertations++;
            } else {
                arr[c] = right[b];
                b++;
                numOfOpertations++;
            }
            c++;
        }

        while (a < len_left) {
            arr[c] = left[a];
            a++;
            c++;
            numOfOpertations++;
        }

        while (b < len_right) {
            arr[c] = right[b];
            b++;
            c++;
            numOfOpertations++;
        }
    }
    return numOfOpertations;
}

template <Traversable Itr, typename Compare>
size_t algorithms::sorting::merge(Itr begin, Itr end, Compare compare) {
    size_t numOfOpertations{};
    const auto size = distance(begin, end);

    if (size <= 1) return numOfOpertations;

    auto mid = next(begin, size / 2);
    merge(begin, mid, compare);
    merge(mid, end, compare);

    auto len_left = distance(begin, mid);
    auto len_right = distance(next(mid), end);
    auto left_end = next(begin, len_left);
    auto right_end = next(next(mid), len_right);

    using iter_type = typename std::iterator_traits<Itr>::value_type;
    std::vector<iter_type> left{begin, left_end};
    std::vector<iter_type> right{mid, right_end};

    auto leftItr = left.begin();
    auto rightItr = right.begin();
    auto arrItr = begin;

    while (leftItr != left.end() && rightItr != right.end()) {
        if (compare(*leftItr, *rightItr)) {
            *arrItr = *leftItr;
            advance(leftItr, 1);
            numOfOpertations++;
        } else {
            *arrItr = *rightItr;
            advance(rightItr, 1);
            numOfOpertations++;
        }
        advance(arrItr, 1);
    }

    while (leftItr != left.end()) {
        *arrItr = *leftItr;
        advance(leftItr, 1);
        advance(arrItr, 1);
        numOfOpertations++;
    }

    while (rightItr != right.end()) {
        *arrItr = *rightItr;
        advance(rightItr, 1);
        advance(arrItr, 1);
        numOfOpertations++;
    }
    return numOfOpertations;
}

template <typename T, size_t n, typename Compare>
size_t algorithms::sorting::shell(T (&arr)[n], Compare compare) {
    size_t numOfOpertations{0};
    int distance = floor(n / 2);
    while (distance > 0) {
        for (auto i = distance; i < n; i++) {
            auto tmp = arr[i];
            auto j = i;
            while (j >= distance && compare(tmp, arr[j - distance])) {
                arr[j] = arr[j - distance];
                numOfOpertations++;
                j -= distance;
            }
            arr[j] = tmp;
            numOfOpertations++;
        }
        distance = floor(distance / 2);
    }
    return numOfOpertations;
}

template <Traversable Itr, typename Compare>
size_t algorithms::sorting::shell(Itr begin, Itr end, Compare compare) {
    size_t numOfOpertations{};
    auto size = distance(begin, end);
    auto h = 1;
    while (h < size / 3) h = 3 * h + 1;
    while (h >= 1) {
        for (auto i = begin + h; i != end; i++) {
            for (auto j = i; (j - begin) >= h && compare(*j, *(j - h)); j -= h) {
                numOfOpertations++;
                std::iter_swap(j, j - h);
            }
        }
        h /= 3;
        numOfOpertations++;
    }
    return numOfOpertations;
}

template <typename T, size_t n>
std::pair<std::optional<size_t>, size_t> algorithms::searching::binary(const T (&arr)[n], const T& target) {
    size_t low = 0;
    size_t high = n - 1;
    size_t found{};
    size_t numOfOpertations{};

    while (low <= high) {
        size_t mid = floor((high + low) / 2);
        auto guess = arr[mid];

        if (guess == target) {
            found = mid;
            numOfOpertations++;
            return {found, numOfOpertations};
        }

        if (guess > target) {
            high = mid - 1;
            numOfOpertations++;
        } else {
            low = mid + 1;
            numOfOpertations++;
        }
    }
    return {std::nullopt, numOfOpertations};
}

template <Traversable Itr>
std::pair<Itr, size_t> algorithms::searching::binary(Itr begin, Itr end, const std::iter_value_t<Itr>& target) {
    auto high = begin;
    auto low = end;
    Itr found{};
    size_t numOfOpertations{};

    while (low != high) {
        auto mid = next(high, (distance(high, low) / 2));
        auto guess = *mid;

        if (guess == target) {
            found = mid;
            numOfOpertations++;
            return {found, numOfOpertations};
        }

        if (target > guess) {
            high = next(mid);
            numOfOpertations++;
        } else {
            low = mid;
            numOfOpertations++;
        }
    }

    return {end, numOfOpertations};
}

template <typename T, size_t n, typename Compare>
size_t algorithms::sorting::quicksort(T (&arr)[n], size_t low, size_t high, Compare compare) {
    size_t numOfOpertations{};
    if (n < 2) return numOfOpertations;
    if (low >= high) return numOfOpertations;

    T pivot{arr[high]};
    long pivotIndex{static_cast<long>(low - 1)};
    for (size_t i{low}; i <= high - 1; i++) {
        // if (arr[i] <= pivot) {
        if (compare(arr[i], pivot)) {
            pivotIndex++;
            T tmp{arr[pivotIndex]};
            arr[pivotIndex] = arr[i];
            arr[i] = tmp;
            numOfOpertations++;
        }
    }
    pivotIndex++;
    T tmp{arr[pivotIndex]};
    arr[pivotIndex] = arr[high];
    arr[high] = tmp;
    numOfOpertations++;

    quicksort(arr, low, pivotIndex - 1, compare);
    quicksort(arr, pivotIndex + 1, high, compare);
    return numOfOpertations;
}

template <Traversable Itr, typename Compare>
size_t algorithms::sorting::quicksort(Itr begin, Itr end, Compare compare) {
    size_t numOfOpertations{};
    if (distance(begin, end) < 1) return numOfOpertations;

    auto pivot = prev(end);
    auto pivotItr = begin;

    for (auto j = begin; j != pivot; ++j) {
        if (compare(*j, *pivot)) {
            std::iter_swap(pivotItr++, j);
            numOfOpertations++;
        }
    }
    std::iter_swap(pivotItr, pivot);
    numOfOpertations++;

    quicksort(begin, pivotItr, compare);
    quicksort(next(pivotItr), end, compare);

    return numOfOpertations;
}
