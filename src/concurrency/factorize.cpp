#include <array>
#include <chrono>
#include <future>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "stopwatch.hpp"
#include "timer.hpp"

namespace chrono = std::chrono;
using std::string, std::stringstream, std::multiset, std::set, std::cout, std::endl, std::vector, std::future,
    std::launch, std::async, std::future_status;

template <typename T>
set<T> factorize(T x) {
    set<T> res{1};

    for (T candidate{2}; candidate <= x; candidate++) {
        if (x % candidate == 0) {
            res.insert(candidate);
            x /= candidate;
            candidate = 1;
        }
    }
    return res;
}

string factor_task(unsigned long long x) {
    chrono::nanoseconds elapsed_ns;
    set<unsigned long long> factors;
    {
        Stopwatch stopwatch{elapsed_ns};
        factors = factorize(x);
    }

    const auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(elapsed_ns).count();
    stringstream ss;
    ss << elapsed_ms << " ms: Factoring " << x << "( ";
    for (auto factor : factors) ss << factor << " ";
    ss << ")\n";

    return ss.str();
}

std::array<unsigned long long, 6> numbers{9'699'690,     179'426'549,   1'000'000'007,
                                          4'294'967'291, 4'294'967'296, 1'307'674'368'000};

int main() {
    // chrono::nanoseconds elapsed_ns;
    {
        // Stopwatch stopwatch{elapsed_ns};
        TimerClass timer{"Timer"};
        vector<future<string>> factor_tasks;
        // for (auto number : numbers) factor_tasks.emplace_back(async(launch::async, factor_task, number));
        auto worker = [](auto number) { return async(launch::async, factor_task, number); };
        std::transform(numbers.begin(), numbers.end(), std::back_inserter(factor_tasks), worker);

        for (auto& task : factor_tasks) cout << task.get();
    }

    // const auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(elapsed_ns).count();
    // cout << elapsed_ms << "ms: total program time" << endl;
}
