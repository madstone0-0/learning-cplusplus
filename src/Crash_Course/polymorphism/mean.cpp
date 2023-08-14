#include <cstddef>
#include <cstdio>

template <typename T, size_t size>
T mean(T (&arr)[size]) {
    T sum{};
    for (T val : arr) {
        sum += val;
    }
    return sum / size;
}

int main() {
    double dArr[]{2.3, 3.4, 3.4, 45.5};
    int iArr[]{3, 44, 23, 54, 23, 54, 23, 5};
    long lArr[]{32323, 323232, 465454, 32325, 4545};
    printf("dArr: %f\n", mean(dArr));
    printf("iArr: %i\n", mean(iArr));
    printf("lArr: %ld\n", mean(lArr));
}
