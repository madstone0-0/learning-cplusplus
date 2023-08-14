#pragma once

#include <chrono>

struct Stopwatch {
    Stopwatch(std::chrono::nanoseconds& result) : result{result}, start{std::chrono::high_resolution_clock::now()} {}
    ~Stopwatch() { result = std::chrono::high_resolution_clock::now() - start; }

   private:
    std::chrono::nanoseconds& result;
    const std::chrono::time_point<std::chrono::high_resolution_clock> start;
};
