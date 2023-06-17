#include <sys/time.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <utility>

struct TimerClass {
    TimerClass(const char *x);
    TimerClass(const TimerClass &other);
    TimerClass(TimerClass &&other) noexcept;
    TimerClass &operator=(const TimerClass &other);
    TimerClass &operator=(TimerClass &&other) noexcept;
    ~TimerClass();

   private:
    timeval *timestamp;
    char *name;
};
