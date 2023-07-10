#ifndef TIMER_INCLUDE
#define TIMER_INCLUDE

#include <sys/time.h>
#include <unistd.h>

#include <boost/smart_ptr/scoped_array.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
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
    boost::scoped_ptr<timeval> timestamp{};
    boost::scoped_array<char> name{};
};

#endif
