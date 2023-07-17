#ifndef TIMER_INCLUDE
#define TIMER_INCLUDE

#include <sys/time.h>
#include <unistd.h>

#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <utility>

struct TimerClass {
    explicit TimerClass(const std::string &x);
    TimerClass(const TimerClass &other);
    TimerClass(TimerClass &&other) noexcept;
    TimerClass &operator=(const TimerClass &other);
    TimerClass &operator=(TimerClass &&other) noexcept;
    ~TimerClass();

    // std::shared_ptr<std::chrono::time_point<std::chrono::high_resolution_clock>> timestamp =
    //     std::make_shared<std::chrono::time_point<std::chrono::high_resolution_clock>>();
    std::shared_ptr<std::chrono::time_point<std::chrono::high_resolution_clock>> timestamp{};
    // std::shared_ptr<char[]> name{};
    std::shared_ptr<std::string> name{};
};

#endif
