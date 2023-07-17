#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>

#include "algorithms.hpp"
#include "timer.hpp"

template <typename T>
void printArray(const T* arr, size_t maxShown) {
    for (auto i = 0; i < maxShown; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "...\n\n";
}

template <typename T, size_t n>
void printArray(const std::array<T, n>& arr, size_t maxShown) {
    for (auto i = 0; i < maxShown; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "...\n\n";
}

void printNumOfOps(size_t ops) { std::cout << "Number of Operations: " << ops << "\n"; }

int main() {
    using namespace algorithms;

    std::mt19937_64 mtEngine{69};
    // const size_t arrayLength{690'000};
    // const size_t arrayLength{1'000'000};
    const size_t arrayLength{100'000};
    // const size_t arrayLength{400'000};
    // const size_t arrayLength{4};
    const size_t maxShown{30};
    std::uniform_int_distribution<int> intD{1, 10000};
    size_t numOfOps{0};

    // int workingList[arrayLength]{};
    // int originalList[arrayLength]{};
    // std::unique_ptr<int[]> workingList{new int[arrayLength]};
    // std::unique_ptr<int[]> originalList{new int[arrayLength]};
    std::array<int, arrayLength> workingList{};
    std::array<int, arrayLength> originalList{};
    // srand(69);

    for (size_t i = 0; i < arrayLength; i++) {
        // originalList[i] = rand() % 10000;
        originalList[i] = intD(mtEngine);
    }

    std::copy(std::begin(originalList), std::end(originalList), std::begin(workingList));

    printf("Dataset size: %zu\nFirst %zu elements:\n", arrayLength, maxShown);
    printArray(workingList, maxShown);

    // TimerClass* bubbleSort = new TimerClass("BubbleSort");
    // bubble(workingList);
    // delete bubbleSort;
    // printArray(workingList, 20);
    // std::copy(std::begin(originalList), std::end(originalList), std::begin(workingList));

    // {
    //     TimerClass insertSort{"InsertSort"};
    //     sorting::insertion(workingList);
    // }
    // printArray(workingList, maxShown);
    // std::copy(std::begin(originalList), std::end(originalList), std::begin(workingList));

    {
        TimerClass mergeSort{"MergeSort"};
        numOfOps = sorting::merge(workingList, 0, arrayLength);
    }
    printNumOfOps(numOfOps);
    printArray(workingList, maxShown);
    std::copy(std::begin(originalList), std::end(originalList), std::begin(workingList));

    {
        TimerClass shellSort{"ShellSort"};
        numOfOps = sorting::shell(workingList);
    }
    printNumOfOps(numOfOps);
    printArray(workingList, maxShown);
    std::copy(std::begin(originalList), std::end(originalList), std::begin(workingList));

    {
        TimerClass quickSort{"QuickSort"};
        numOfOps = sorting::quicksort(workingList, 0, arrayLength);
    }
    printNumOfOps(numOfOps);
    printArray(workingList, maxShown);
    std::copy(std::begin(originalList), std::end(originalList), std::begin(workingList));

    std::optional<size_t> res{};
    {
        sorting::quicksort(workingList, 0, arrayLength);
        TimerClass binarySearch{"BinarySearch"};
        auto [result, ops] = searching::binary(workingList, 5032);
        res = result;
        numOfOps = ops;
    }
    auto resultString = res.has_value() ? std::to_string(res.value()) : "Not found";
    printNumOfOps(numOfOps);
    std::cout << "Result: " << resultString << "\n";
    std::copy(std::begin(originalList), std::end(originalList), std::begin(workingList));

    return 0;
}
