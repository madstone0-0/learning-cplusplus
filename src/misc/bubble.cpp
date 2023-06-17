#include <cstdlib>
#include <iostream>
#include <utility>

#include "timer.cpp"

using std::cout;

template <typename T, size_t n>
T* bubble(const T (&arr)[n]) {
    T* sorted = new T[n];
    for (size_t i = 0; i < n; i++) {
        sorted[i] = arr[i];
    }

    size_t len = n - 1;
    for (size_t i = len; i > 0; i--) {
        for (size_t j = 0; j < i; j++) {
            if (sorted[j - 1] > sorted[j]) {
                T temp = sorted[j];
                sorted[j] = sorted[j - 1];
                sorted[j - 1] = temp;
            }
        }
    }
    return sorted;
}

int main() {
    // int list[] = {1, 3, 6, 2, 7};
    // int* sorted = bubble(list);
    // for (size_t i = 0; i < 5; i++) {
    //     cout << sorted[i] << " ";
    // }
    // cout << "\n";
    // delete[] sorted;
    //
    // double list2[] = {1, 3, 6, 2, 7};
    // double* sorted2 = bubble(list2);
    // for (size_t i = 0; i < 5; i++) {
    //     cout << sorted2[i] << " ";
    // }
    // cout << "\n";
    // delete[] sorted2;

    srand(100);
    int list[1000]{};
    for (size_t i = 0; i < 1000; i++) {
        list[i] = rand() % 1000;
    }

    TimerClass* bubbleSortTimer = new TimerClass{"BubbleSortTimer"};
    int* sorted = bubble(list);
    delete bubbleSortTimer;

    for (size_t i = 0; i < 50; i++) {
        cout << sorted[i] << " ";
    }
    cout << "...\n";

    delete[] sorted;
    return 0;
}
