#include <algorithm>
#include <iostream>
#include <iterator>

#include "algorithms.hpp"
#include "timer.hpp"

template <typename T>
void printArray(T* arr, size_t maxShown) {
    for (auto i = 0; i < maxShown; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "...\n\n";
}

int main() {
    using namespace Algorithms;
    srand(42);
    const size_t arrayLength{100'000};
    int workingList[arrayLength]{};
    int originalList[arrayLength]{};
    // int list1[arrayLength]{};
    // int list2[arrayLength]{};
    // int list3[arrayLength]{};
    // int list4[arrayLength]{};

    for (size_t i = 0; i < arrayLength; i++) {
        originalList[i] = rand() % arrayLength;
        // list1[i] = rand() % arrayLength * 2;
        // list2[i] = rand() % arrayLength * 2;
        // list3[i] = rand() % arrayLength * 2;
        // list4[i] = rand() % arrayLength * 2;
    }

    std::copy(std::begin(originalList), std::end(originalList), std::begin(workingList));

    printf("Dataset size: %zu\n=====\n\n", arrayLength);
    TimerClass* bubbleSort = new TimerClass("BubbleSort");
    bubble(workingList);
    delete bubbleSort;
    printArray(workingList, 20);
    std::copy(std::begin(originalList), std::end(originalList), std::begin(workingList));

    TimerClass* insertSort = new TimerClass("InsertSort");
    insertion(workingList);
    delete insertSort;
    printArray(workingList, 20);
    std::copy(std::begin(originalList), std::end(originalList), std::begin(workingList));

    TimerClass* mergeSort = new TimerClass("MergeSort");
    merge(workingList, 0, sizeof(workingList) / sizeof(workingList[0]) - 1);
    delete mergeSort;
    printArray(workingList, 20);
    std::copy(std::begin(originalList), std::end(originalList), std::begin(workingList));

    TimerClass* shellSort = new TimerClass("ShellSort");
    shell(workingList);
    delete shellSort;
    printArray(workingList, 20);
    std::copy(std::begin(originalList), std::end(originalList), std::begin(workingList));

    return 0;
}
