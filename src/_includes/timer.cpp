#include "timer.hpp"

using std::strncpy, std::cout;

template <typename T>
constexpr T abs(T x) {
    return x < 0 ? x * -1 : x;
}

TimerClass::TimerClass(const char *x) : timestamp{new timeval{}}, name{new char[strlen(x)]{}} {
    strncpy(name, x, strlen(x));
    gettimeofday(timestamp, NULL);
}

TimerClass::TimerClass(const TimerClass &other) : timestamp{new timeval{}}, name{new char[strlen(other.name)]{}} {
    strncpy(name, other.name, strlen(other.name));
    timestamp->tv_sec = other.timestamp->tv_sec;
    timestamp->tv_usec = other.timestamp->tv_usec;
}

TimerClass::TimerClass(TimerClass &&other) noexcept : timestamp{other.timestamp}, name{other.name} {
    other.timestamp = nullptr;
    other.name = nullptr;
}

TimerClass &TimerClass::operator=(const TimerClass &other) {
    if (this == &other) return *this;
    const auto newTimestamp = new timeval{};
    delete timestamp;
    timestamp = newTimestamp;
    timestamp->tv_sec = other.timestamp->tv_sec;
    timestamp->tv_usec = other.timestamp->tv_usec;

    const auto newName = new char[strlen(other.name)]{};
    delete[] name;
    name = newName;
    strncpy(name, other.name, strlen(other.name));

    return *this;
}

TimerClass &TimerClass::operator=(TimerClass &&other) noexcept {
    if (this == &other) return *this;

    delete timestamp;
    delete[] name;

    timestamp = other.timestamp;
    name = other.name;

    other.timestamp = nullptr;
    other.name = nullptr;
    return *this;
}

TimerClass::~TimerClass() {
    timeval *currTime = new timeval{};
    gettimeofday(currTime, NULL);
    if (!(timestamp == nullptr)) {
        if ((currTime->tv_usec - timestamp->tv_usec) == 0) {
            cout << "(" << name << ")"
                 << " age: " << (currTime->tv_usec - timestamp->tv_usec) << " microseconds\n";
        } else {
            cout << "(" << name << ")"
                 << " age: " << (currTime->tv_sec - timestamp->tv_sec) << " seconds "
                 << abs<__suseconds_t>(currTime->tv_usec - timestamp->tv_usec) << " microseconds\n";
        }
    }
    delete timestamp;
    delete currTime;
    delete[] name;
}
