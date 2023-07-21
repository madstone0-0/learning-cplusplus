#include <cstdio>
#include <forward_list>
#include <iterator>
#include <random>
#include <vector>

#include "timer.hpp"

int rng() {
    std::mt19937 mtEngine{42};
    std::uniform_int_distribution<int> intD{1, 1000};
    return intD(mtEngine);
}

int main() {
    size_t datasetSize{100'000};
    const size_t distance{static_cast<size_t>(std::floor(datasetSize / 2))};
    std::forward_list<int> forwardList;
    std::vector<int> vec;

    printf("Dataset size: %zu\nAdvance distance: %zu\n", datasetSize, distance);
    while (datasetSize--) {
        forwardList.push_front(rng());
        vec.push_back(rng());
    }

    {
        TimerClass vectorTimer{"Vector"};
        auto itr = vec.begin();
        std::advance(itr, distance);
    }

    {
        TimerClass forwardListTimer{"Forward List"};
        auto itr = forwardList.begin();
        std::advance(itr, distance);
    }

    return 0;
}
