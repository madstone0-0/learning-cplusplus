#include <algorithm>
#include <future>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>
#include <ranges>
#include <string>

#include "algorithms.hpp"
#include "concepts.hpp"
#include "timer.hpp"

// using mts::concepts::Traversable;
using std::future, std::async, std::launch, std::cout;
namespace ranges = std::ranges;

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

template <typename Itr>
    requires std::random_access_iterator<Itr>
void resetArray(Itr sourceBegin, Itr sourceEnd, Itr targetBegin) {
    std::copy(sourceBegin, sourceEnd, targetBegin);
}

template <typename T>
bool isReady(const future<T>& f) {
    if (f.valid()) {
        return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    } else {
        return false;
    }
}

int main() {
    using std::less;
    using namespace algorithms;

    std::mt19937_64 mtEngine{69};
    constexpr size_t arrayLength{10'000'000};
    // constexpr size_t arrayLength{1'000'000};
    const size_t maxShown{30};
    volatile size_t numOfOps{0};

    struct AlgoResult {
        std::string name;
        std::chrono::nanoseconds elapsed;
        size_t numOfOps;
        std::vector<int> sorted;
    };

    // std::array<int, arrayLength> workingList{};
    std::vector<int> workingList(arrayLength);

    std::iota(workingList.begin(), workingList.end(), 1);
    // sanges::iota(workingList, 1)

    ranges::shuffle(workingList, mtEngine);

    // printf("Dataset size: %zu\nFirst %zu elements:\n", arrayLength, maxShown);
    std::cout << "Dataset size: " << arrayLength << "\nFirst " << maxShown << " elements "
              << ":\n";
    printArray(workingList.begin(), workingList.end(), maxShown);

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

    std::vector<future<AlgoResult>> futures;
    std::vector<AlgoResult> completed;

    // futures.push_back(async(launch::async, [&] {
    //     std::chrono::nanoseconds elapsed{};
    //     size_t numOfOps{};
    //     decltype(workingList) dataCopy{workingList};
    //     {
    //         TimerClass timer{"BubbleSort", &elapsed};
    //         numOfOps = sorting::bubble(dataCopy.begin(), dataCopy.end(), less());
    //     }
    //     return AlgoResult{"BubbleSort", elapsed, numOfOps, {dataCopy.begin(), dataCopy.begin() + maxShown}};
    // }));

    // {
    //     TimerClass mergeSort{"MergeSort"};
    //     numOfOps = sorting::merge(workingList.begin(), workingList.end(), less());
    // }
    // printNumOfOps(numOfOps);
    // printArray(workingList.begin(), workingList.end(), maxShown);
    // std::shuffle(workingList.begin(), workingList.end(), mtEngine);

    // futures.push_back(async(launch::async, [&] {
    //     std::chrono::nanoseconds elapsed{};
    //     size_t numOfOps{};
    //     decltype(workingList) dataCopy{workingList};
    //     {
    //         TimerClass timer{"InsertSort", &elapsed};
    //         numOfOps = sorting::insertion(dataCopy.begin(), dataCopy.end(), less());
    //     }
    //     return AlgoResult{"InsertSort", elapsed, numOfOps, {dataCopy.begin(), dataCopy.begin() + maxShown}};
    // }));

    futures.push_back(async(launch::async, [&] {
        std::chrono::nanoseconds elapsed{};
        size_t numOfOps{};
        decltype(workingList) dataCopy{workingList};
        {
            TimerClass timer{"MergeSort", &elapsed};
            numOfOps = sorting::merge(dataCopy.begin(), dataCopy.end(), less());
        }
        return AlgoResult{"MergeSort", elapsed, numOfOps, {dataCopy.begin(), dataCopy.begin() + maxShown}};
    }));

    // {
    //     TimerClass shellSort{"ShellSort"};
    //     numOfOps = sorting::shell(workingList.begin(), workingList.end(), less());
    // }
    // printNumOfOps(numOfOps);
    // printArray(workingList.begin(), workingList.end(), maxShown);
    // std::shuffle(workingList.begin(), workingList.end(), mtEngine);

    futures.push_back(async(launch::async, [&] {
        std::chrono::nanoseconds elapsed{};
        size_t numOfOps{};
        decltype(workingList) dataCopy{workingList};
        {
            TimerClass timer{"ShellSort", &elapsed};
            numOfOps = sorting::shell(dataCopy.begin(), dataCopy.end(), less());
        }
        return AlgoResult{"ShellSort", elapsed, numOfOps, {dataCopy.begin(), dataCopy.begin() + maxShown}};
    }));

    // {
    //     TimerClass quickSort{"QuickSort"};
    //     numOfOps = sorting::quicksort(workingList.begin(), workingList.end(), less());
    // }
    // printNumOfOps(numOfOps);
    // printArray(workingList.begin(), workingList.end(), maxShown);
    // std::shuffle(workingList.begin(), workingList.end(), mtEngine);

    futures.push_back(async(launch::async, [&] {
        std::chrono::nanoseconds elapsed{};
        size_t numOfOps{};
        decltype(workingList) dataCopy{workingList};
        {
            TimerClass timer{"QuickSort", &elapsed};
            numOfOps = sorting::quicksort(dataCopy.begin(), dataCopy.end(), less());
        }
        return AlgoResult{"QuickSort", elapsed, numOfOps, {dataCopy.begin(), dataCopy.begin() + maxShown}};
    }));

    decltype(workingList)::iterator resItr{};
    {
        sorting::quicksort(workingList.begin(), workingList.end(), less());
        TimerClass binarySearch{"BinarySearch"};
        auto [result, ops] = searching::binary(workingList.begin(), workingList.end(), 5032);

        resItr = result;
        numOfOps = ops;
    }
    auto resultString = !(resItr == workingList.end()) ? std::to_string(*resItr) : "Not found";
    printNumOfOps(numOfOps);
    std::cout << "Result: " << resultString << "\n\n";

    try {
        do {
            for (auto& future : futures) {
                if (isReady(future)) {
                    auto res = future.get();
                    const auto& [name, elapsed, numOfOps, sorted] = res;
                    auto inSeconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed);
                    auto inMicroSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed);
                    cout << "(" << name << "): age ";
                    if (inSeconds.count() <= 0)
                        cout << inMicroSeconds << "\n";
                    else
                        cout << inSeconds << "\n";
                    printNumOfOps(numOfOps);
                    printArray(sorted.begin(), sorted.end(), maxShown);

                    completed.push_back(res);
                };
            }
        } while (completed.size() < futures.size());

        // for (const auto& [name, elapsed, numOfOps, sorted] : completed) {
        //     auto inSeconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed);
        //     auto inMicroSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed);
        //     cout << "(" << name << "): age ";
        //     if (inSeconds.count() <= 0)
        //         cout << inMicroSeconds << "\n";
        //     else
        //         cout << inSeconds << "\n";
        //     printNumOfOps(numOfOps);
        //     printArray(sorted.begin(), sorted.end(), maxShown);
        // }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
