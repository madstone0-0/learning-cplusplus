#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "timer.hpp"
// #include "stopwatch.hpp"

using namespace std;

vector<long> make_random_vector() {
    // vector<long> numbers(1'000'000'000);
    vector<long> numbers(10'000'000);
    iota(numbers.begin(), numbers.end(), 0);
    mt19937_64 urng{121216};
    shuffle(numbers.begin(), numbers.end(), urng);
    return numbers;
}

int main() {
    cout << "Constructing random vectors...";
    auto numbers_a = make_random_vector();
    auto numbers_b{numbers_a};
    cout << " " << numbers_a.size() << " elements\n";
    cout << "Sorting with execution::seq...";
    {
        TimerClass seq{"Seq"};
        sort(execution::seq, numbers_a.begin(), numbers_a.end());
    }

    cout << "Sorting with execution::par...";
    {
        TimerClass par{"Par"};
        sort(execution::par, numbers_b.begin(), numbers_b.end());
    }

    return 0;
}
