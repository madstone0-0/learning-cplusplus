#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "timer.cpp"

using std::cout;

struct Tracer {
    Tracer(const char* name) : name{name} { printf("%s constructed\n", name); }

    ~Tracer() { printf("%s destructed\n", name); }

   private:
    const char* const name;
};

template <typename T>
struct Return {
    T* value{};
    Tracer* tracer{};
};

template <typename T, size_t n>
T* insertion(const T (&arr)[n]) {
    T* sorted = new T[n];
    for (size_t i = 0; i < n; i++) {
        sorted[i] = arr[i];
    }

    for (size_t i = 1; i < n; i++) {
        size_t j = i - 1;
        T element_next = sorted[i];
        while (sorted[j] > element_next && j >= 0) {
            sorted[j + 1] = sorted[j];
            j--;
        }
        sorted[j + 1] = element_next;
    }
    return sorted;
}

int main() {
    srand(100);
    int listInts[1000]{};
    for (size_t i = 0; i < 1000; i++) {
        listInts[i] = rand() % 1000;
    }

    TimerClass* insertSortTimer = new TimerClass{"InsertSort"};
    int* sortedInts = insertion(listInts);
    delete insertSortTimer;

    for (size_t i = 0; i < 50; i++) {
        cout << sortedInts[i] << " ";
    }
    cout << "...\n";
    //
    // long listLongs[50]{};
    // for (size_t i = 0; i < 50; i++) {
    //     listLongs[i] = rand() % 100000;
    // }
    // long* sortedLongs = insertion(listLongs);
    //
    // for (size_t i = 0; i < 50; i++) {
    //     cout << sortedLongs[i] << " ";
    // }
    // cout << "\n";

    delete[] sortedInts;
    // delete[] sortedLongs;

    return 0;
}
