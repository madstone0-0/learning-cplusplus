#ifndef TIMER_INCLUDE
#define TIMER_INCLUDE

#include <sys/time.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <utility>

struct TimerClass {
    explicit TimerClass(const char *x);
    TimerClass(const TimerClass &other);
    TimerClass(TimerClass &&other) noexcept;
    TimerClass &operator=(const TimerClass &other);
    TimerClass &operator=(TimerClass &&other) noexcept;
    ~TimerClass();

    // std::shared_ptr<std::chrono::time_point<std::chrono::high_resolution_clock>> timestamp =
    //     std::make_shared<std::chrono::time_point<std::chrono::high_resolution_clock>>();
    std::shared_ptr<std::chrono::time_point<std::chrono::high_resolution_clock>> timestamp{};
    std::shared_ptr<char[]> name{};
};

#endif
