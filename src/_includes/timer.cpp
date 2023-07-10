#include "timer.hpp"

#include <boost/smart_ptr/scoped_array.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>

using std::strncpy, std::cout, boost::scoped_ptr, boost::scoped_array;

template <typename T>
constexpr T abs(T x) {
    return x < 0 ? x * -1 : x;
}

TimerClass::TimerClass(const char *x) : timestamp{new timeval{}}, name{new char[strlen(x)]{}} {
    strncpy(name.get(), x, strlen(x));
    gettimeofday(timestamp.get(), NULL);
}

TimerClass::TimerClass(const TimerClass &other) : timestamp{new timeval{}}, name{new char[strlen(other.name.get())]{}} {
    strncpy(name.get(), other.name.get(), strlen(other.name.get()));
    timestamp->tv_sec = other.timestamp->tv_sec;
    timestamp->tv_usec = other.timestamp->tv_usec;
}

TimerClass::TimerClass(TimerClass &&other) noexcept {
    timestamp.reset(other.timestamp.get());
    name.reset(other.name.get());
    other.timestamp.reset();
    other.name.reset();
}

TimerClass &TimerClass::operator=(const TimerClass &other) {
    if (this == &other) return *this;
    timestamp.reset(other.timestamp.get());

    name.reset(other.name.get());

    return *this;
}

TimerClass &TimerClass::operator=(TimerClass &&other) noexcept {
    if (this == &other) return *this;

    name.reset(other.name.get());
    timestamp.reset(other.timestamp.get());

    other.timestamp.reset();
    other.timestamp.reset();
    return *this;
}

TimerClass::~TimerClass() {
    scoped_ptr<timeval> currTime{new timeval{}};
    gettimeofday(currTime.get(), NULL);

    if (!(timestamp == nullptr)) {
        if ((currTime->tv_usec - timestamp->tv_usec) == 0) {
            cout << "(" << name.get() << ")"
                 << " age: " << (currTime->tv_usec - timestamp->tv_usec) << " microseconds\n";
        } else {
            cout << "(" << name.get() << ")"
                 << " age: " << (currTime->tv_sec - timestamp->tv_sec) << " seconds "
                 << abs<__suseconds_t>(currTime->tv_usec - timestamp->tv_usec) << " microseconds\n";
        }
    }
}
