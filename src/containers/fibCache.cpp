#include <chrono>
#include <cstdio>
// #include <map>
#include <random>
#include <unordered_map>

long fib_sum(size_t n) {
    long sum{};
    long last = 0;
    long current = 1;
    while (n--) {
        auto tmp = current;
        current += last;
        last = tmp;
        sum += current;
    }
    return sum;
}

long random() {
    static std::mt19937_64 mt_engine{102787};
    static std::uniform_int_distribution<long> int_d{1000, 2000};
    return int_d(mt_engine);
}

struct Stopwatch {
    Stopwatch(std::chrono::nanoseconds& result) : result{result}, start{std::chrono::system_clock::now()} {}
    ~Stopwatch() { result = std::chrono::system_clock::now() - start; }

   private:
    std::chrono::nanoseconds& result;
    const std::chrono::time_point<std::chrono::system_clock> start;
};

long cached_fib_sum(const size_t& n) {
    static std::unordered_map<long, long> cache;

    if (cache.contains(n)) {
        return cache[n];
    } else {
        cache[n] = fib_sum(n);
        return cache[n];
    }
}

int main() {
    size_t samples{1'000'000};
    std::chrono::nanoseconds elapsed;

    {
        Stopwatch stopwatch{elapsed};
        volatile double answer;
        while (samples--) {
            // answer = fib_sum(random());
            answer = cached_fib_sum(random());
        }
    }
    printf("Elapsed: %g s.\n", elapsed.count() / 1'000'000'000.);
}
