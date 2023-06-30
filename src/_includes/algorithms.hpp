#ifndef ALGORITHMS_INCLUDE
#define ALGORITHMS_INCLUDE

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

template <typename T, size_t n>
void Algorithms::bubble(T (&arr)[n]) {
    size_t length = n - 1;
    for (auto i = length; i > 0; i--) {
        for (auto j = 0; j < i; j++) {
            if (arr[j - 1] > arr[j]) {
                T tmp = arr[j];
                arr[j] = arr[j - 1];
                arr[j - 1] = tmp;
            }
        }
    }
}

template <typename T, size_t n>
void Algorithms::insertion(T (&arr)[n]) {
    for (auto i = 1; i < n; i++) {
        size_t j = i - 1;
        T element_next = arr[i];
        while (arr[j] > element_next && j >= 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = element_next;
    }
}

template <typename T, size_t n>
void Algorithms::merge(T (&arr)[n], size_t start, size_t end) {
    if (start < end) {
        size_t mid = start + (end - start) / 2;
        merge(arr, start, mid);
        merge(arr, mid + 1, end);

        // Merge
        size_t len_left = mid - start + 1;
        size_t len_right = end - mid;

        T left[len_left], right[len_right];

        for (auto i = 0; i < len_left; i++) {
            left[i] = arr[start + i];
        }

        for (auto i = 0; i < len_right; i++) {
            right[i] = arr[mid + 1 + i];
        }

        size_t a = 0;
        size_t b = 0;
        size_t c = start;

        while (a < len_left && b < len_right) {
            if (left[a] < right[b]) {
                arr[c] = left[a];
                a++;
            } else {
                arr[c] = right[b];
                b++;
            }
            c++;
        }

        while (a < len_left) {
            arr[c] = left[a];
            a++;
            c++;
        }

        while (b < len_right) {
            arr[c] = right[b];
            b++;
            c++;
        }
    }
}

template <typename T, size_t n>
void Algorithms::shell(T (&arr)[n]) {
    int distance = floor(n / 2);
    while (distance > 0) {
        for (auto i = distance; i < n; i++) {
            auto tmp = arr[i];
            auto j = i;
            while (j >= distance && arr[j - distance] > tmp) {
                arr[j] = arr[j - distance];
                j -= distance;
            }
            arr[j] = tmp;
        }
        distance = floor(distance / 2);
    }
}

#endif
