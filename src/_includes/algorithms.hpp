#ifndef ALGORITHMS_INCLUDE
#define ALGORITHMS_INCLUDE

#include <array>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "utils.hpp"

namespace algorithms {

    namespace sorting {
        template <typename T, size_t n>
        size_t bubble(T (&arr)[n]);

        template <typename T, size_t n>
        size_t insertion(T (&arr)[n]);

        template <typename T, size_t n>
        size_t merge(T (&arr)[n], size_t start, size_t end);

        template <typename T, size_t n>
        size_t merge(std::array<T, n>& arr, size_t start, size_t end);

        template <typename T, size_t n>
        size_t shell(T (&arr)[n]);

        template <typename T, size_t n>
        size_t shell(std::array<T, n>& arr);

        template <typename T, size_t n>
        size_t quicksort(T (&arr)[n], size_t low, size_t high);

        template <typename T, size_t n>
        size_t quicksort(std::array<T, n>& arr, int low, int high);

    }  // namespace sorting

    namespace searching {
        template <typename T, size_t n>
        std::pair<std::optional<size_t>, size_t> binary(const T (&arr)[n], const T& target);

        template <typename T, size_t n>
        std::pair<std::optional<size_t>, size_t> binary(const std::array<T, n>& arr, const T& target);

    }  // namespace searching

}  // namespace algorithms
#endif

template <typename T, size_t n>
size_t algorithms::sorting::bubble(T (&arr)[n]) {
    size_t numOfOpertations{0};
    size_t length = n - 1;
    for (auto i = length; i > 0; i--) {
        for (auto j = 0; j < i; j++) {
            if (arr[j - 1] > arr[j]) {
                T tmp = arr[j];
                arr[j] = arr[j - 1];
                arr[j - 1] = tmp;
                numOfOpertations++;
            }
        }
    }
    return numOfOpertations;
}

template <typename T, size_t n>
size_t algorithms::sorting::insertion(T (&arr)[n]) {
    size_t numOfOpertations{0};
    for (auto i = 1; i < n; i++) {
        size_t j = i - 1;
        T element_next = arr[i];
        while (arr[j] > element_next && j >= 0) {
            arr[j + 1] = arr[j];
            j--;
            numOfOpertations++;
        }
        arr[j + 1] = element_next;
        numOfOpertations++;
    }
    return numOfOpertations;
}

template <typename T, size_t n>
size_t algorithms::sorting::merge(T (&arr)[n], size_t start, size_t end) {
    size_t numOfOpertations{0};
    if (start < end) {
        size_t mid = start + (end - start) / 2;
        merge(arr, start, mid);
        merge(arr, mid + 1, end);

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
            if (left[a] < right[b]) {
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

template <typename T, size_t n>
size_t algorithms::sorting::merge(std::array<T, n>& arr, size_t start, size_t end) {
    size_t numOfOpertations{};
    if (start < end) {
        size_t mid = std::floor((start + end) / 2);
        merge(arr, start, mid);
        merge(arr, mid + 1, end);

        // Merging
        size_t len_left = mid - start + 1;
        size_t len_right = end - mid;

        std::vector<T> left{arr.begin() + start, arr.begin() + start + len_left};
        std::vector<T> right{arr.begin() + mid + 1, arr.begin() + mid + 1 + len_right};

        size_t a = 0;
        size_t b = 0;
        size_t c = start;

        while (a < len_left && b < len_right) {
            if (left.at(a) < right.at(b)) {
                arr.at(c) = left.at(a);
                a++;
                numOfOpertations++;
            } else {
                arr.at(c) = right.at(b);
                b++;
                numOfOpertations++;
            }
            c++;
        }

        while (a < len_left) {
            arr.at(c) = left.at(a);
            a++;
            c++;
            numOfOpertations++;
        }

        while (b < len_right) {
            arr.at(c) = right.at(b);
            b++;
            c++;
            numOfOpertations++;
        }
    }
    return numOfOpertations;
}

template <typename T, size_t n>
size_t algorithms::sorting::shell(T (&arr)[n]) {
    size_t numOfOpertations{0};
    int distance = floor(n / 2);
    while (distance > 0) {
        for (auto i = distance; i < n; i++) {
            auto tmp = arr[i];
            auto j = i;
            while (j >= distance && arr[j - distance] > tmp) {
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

template <typename T, size_t n>
size_t algorithms::sorting::shell(std::array<T, n>& arr) {
    size_t numOfOpertations{};
    int distance = floor(n / 2);
    while (distance > 0) {
        for (auto i = distance; i < n; i++) {
            auto tmp = arr.at(i);
            auto j = i;
            while (j >= distance && arr.at(j - distance) > tmp) {
                arr.at(j) = arr.at(j - distance);
                numOfOpertations++;
                j -= distance;
            }
            arr.at(j) = tmp;
            numOfOpertations++;
        }
        distance = floor(distance / 2);
    }
    return numOfOpertations;
}

template <typename T, size_t n>
std::pair<std::optional<size_t>, size_t> algorithms::searching::binary(const std::array<T, n>& arr, const T& target) {
    auto low = 0;
    auto high = n - 1;
    size_t numOfOpertations{};
    size_t found{};

    while (low <= high) {
        auto mid = floor((high + low) / 2);
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

template <typename T, size_t n>
size_t algorithms::sorting::quicksort(T (&arr)[n], size_t low, size_t high) {
    size_t numOfOpertations{};
    if (n < 2) return numOfOpertations;
    if (low >= high) return numOfOpertations;

    T pivot{arr[high]};
    long pivotIndex{static_cast<long>(low - 1)};
    for (size_t i{low}; i <= high - 1; i++) {
        if (arr[i] <= pivot) {
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

    quicksort(arr, low, pivotIndex - 1);
    quicksort(arr, pivotIndex + 1, high);
    return numOfOpertations;
}

template <typename T, size_t n>
size_t algorithms::sorting::quicksort(std::array<T, n>& arr, int low, int high) {
    size_t numOfOpertations{};
    if (n < 2) return numOfOpertations;
    if (low >= high) return numOfOpertations;

    T pivot = arr.at(high);
    auto pivotIndex{static_cast<int>(low - 1)};
    for (auto i{low}; i <= high - 1; i++) {
        if (arr[i] <= pivot) {
            pivotIndex++;
            std::swap(arr[pivotIndex], arr[i]);
            numOfOpertations++;
        }
    }
    pivotIndex++;
    std::swap(arr[pivotIndex], arr[high]);
    numOfOpertations++;

    quicksort(arr, low, static_cast<int>(pivotIndex - 1));
    quicksort(arr, pivotIndex + 1, high);

    return numOfOpertations;
}
