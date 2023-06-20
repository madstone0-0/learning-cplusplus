#include <cmath>
#include <iostream>

#include "timer.cpp"

using std::cout;

template <typename T, size_t n>
void mergeSort(T (&arr)[n], size_t start, size_t end) {
    if (start < end) {
        size_t mid = start + (end - start) / 2;
        mergeSort(arr, start, mid);
        mergeSort(arr, mid + 1, end);

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

int main() {
    srand(100);
    int listInts[1000]{};
    for (size_t i = 0; i < 1000; i++) {
        listInts[i] = rand() % 1000;
    }
    size_t len = sizeof(listInts) / sizeof(listInts[0]) - 1;
    TimerClass* mergeSortTimer = new TimerClass{"MergeSort"};
    mergeSort(listInts, 0, len);
    delete mergeSortTimer;
    for (auto i = 0; i < 50; i++) {
        cout << listInts[i] << " ";
    }
    cout << "\n";
    return 0;
}
