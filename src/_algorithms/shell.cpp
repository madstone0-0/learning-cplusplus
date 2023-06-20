#include <cmath>
#include <iostream>

#include "timer.cpp"

using std::cout;

template <typename T, size_t n>
void shell(T (&arr)[n]) {
    int distance = floor(n / 2);
    while (distance > 0) {
        for (auto i = distance; i < n; i++) {
            auto temp = arr[i];
            auto j = i;
            while (j >= distance && arr[j - distance] > temp) {
                arr[j] = arr[j - distance];
                j -= distance;
            }

            arr[j] = temp;
        }
        distance = floor(distance / 2);
    }
}

int main() {
    srand(100);
    int listInts[1000]{};
    for (size_t i = 0; i < 1000; i++) {
        listInts[i] = rand() % 1000;
    }
    TimerClass* shellTimer = new TimerClass{"ShellSort"};
    shell(listInts);
    delete shellTimer;
    for (auto i = 0; i < 50; i++) {
        cout << listInts[i] << " ";
    }
    cout << "\n";

    return 0;
}
