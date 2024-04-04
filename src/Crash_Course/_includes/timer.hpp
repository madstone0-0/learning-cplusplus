#pragma once

#include <sys/time.h>
#include <unistd.h>

#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

struct TimerClass {
    explicit TimerClass(const std::string &x);
    TimerClass(const std::string &x, std::chrono::nanoseconds *duration);
    TimerClass(const std::string &x, std::chrono::nanoseconds *duration, const bool &print);
    TimerClass(const TimerClass &other);
    TimerClass(TimerClass &&other) noexcept;
    TimerClass &operator=(const TimerClass &other);
    TimerClass &operator=(TimerClass &&other) noexcept;
    ~TimerClass();

    std::shared_ptr<std::chrono::time_point<std::chrono::high_resolution_clock>> timestamp{};
    std::shared_ptr<std::string> name{};
    std::chrono::nanoseconds *duration{};
    bool ptrPrint{};
};
