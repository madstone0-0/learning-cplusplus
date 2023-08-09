#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>

#include "algorithms.hpp"
#include "concepts.hpp"
#include "timer.hpp"

// using mts::concepts::Traversable;

template <typename Itr>
    requires std::random_access_iterator<Itr>
void printArray(Itr begin, Itr end, const size_t& maxShown) {
    size_t count{};
    while (begin != end && count <= maxShown) {
        std::cout << *begin << " ";
        std::advance(begin, 1);
        count++;
    }

    std::cout << "\n\n";
}

void printNumOfOps(volatile const size_t& ops) { std::cout << "Number of Operations: " << ops << "\n"; }

template <typename T, size_t n>
void resetArray(std::array<T, n>& origin, std::array<T, n>& working) {
    std::copy(std::begin(origin), std::end(origin), std::begin(working));
}

template <typename Itr>
    requires std::random_access_iterator<Itr>
void resetArray(Itr sourceBegin, Itr sourceEnd, Itr targetBegin) {
    std::copy(sourceBegin, sourceEnd, targetBegin);
}

int main() {
    using std::less;
    using namespace algorithms;

    std::mt19937_64 mtEngine{69};
    // const size_t arrayLength{690'000};
    // const size_t arrayLength{1'000'000};
    // const size_t arrayLength{100'000};
    const size_t arrayLength{240'000};
    // const size_t arrayLength{400'000};
    // const size_t arrayLength{4};
    const size_t maxShown{30};
    std::uniform_int_distribution<int> intD{1, 100'000};
    volatile size_t numOfOps{0};

    const auto rng = [&intD, &mtEngine]() { return intD(mtEngine); };

    std::array<int, arrayLength> workingList{};
    std::array<int, arrayLength> originalList{};

    std::generate(originalList.begin(), originalList.end(), rng);

    std::copy(std::begin(originalList), std::end(originalList), std::begin(workingList));

    printf("Dataset size: %zu\nFirst %zu elements:\n", arrayLength, maxShown);
    printArray(workingList.begin(), workingList.end(), maxShown);
    resetArray(originalList, workingList);

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
        numOfOps = sorting::merge(workingList.begin(), workingList.end(), less());
    }
    printNumOfOps(numOfOps);
    printArray(workingList.begin(), workingList.end(), maxShown);
    std::shuffle(workingList.begin(), workingList.end(), mtEngine);

    {
        TimerClass shellSort{"ShellSort"};
        numOfOps = sorting::shell(workingList.begin(), workingList.end(), less());
    }
    printNumOfOps(numOfOps);
    printArray(workingList.begin(), workingList.end(), maxShown);
    std::shuffle(workingList.begin(), workingList.end(), mtEngine);

    {
        TimerClass quickSort{"QuickSort"};
        numOfOps = sorting::quicksort(workingList.begin(), workingList.end(), less());
    }
    printNumOfOps(numOfOps);
    printArray(workingList.begin(), workingList.end(), maxShown);
    std::shuffle(workingList.begin(), workingList.end(), mtEngine);

    decltype(workingList)::iterator resItr{};
    {
        sorting::quicksort(workingList.begin(), workingList.end(), std::less<int>());
        TimerClass binarySearch{"BinarySearch"};
        auto [result, ops] = searching::binary(workingList.begin(), workingList.end(), 5032);

        resItr = result;
        numOfOps = ops;
    }
    auto resultString = !(resItr == workingList.end()) ? std::to_string(*resItr) : "Not found";
    printNumOfOps(numOfOps);
    std::cout << "Result: " << resultString << "\n";
    std::shuffle(workingList.begin(), workingList.end(), mtEngine);

    return 0;
}
