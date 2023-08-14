#include <sys/time.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <utility>

template <typename T>
T abs(T x) {
    return x > 0 ? x : x * -1;
}

struct TimerClass {
    TimerClass(const char *x)
        : timestamp{new timeval}, name{new char[strlen(x)]} {
        std::strncpy(name, x, strlen(x));
        gettimeofday(timestamp, NULL);
    };

    TimerClass(const TimerClass &other)
        : timestamp{new timeval}, name{new char[strlen(other.name)]} {
        std::strncpy(name, other.name, strlen(other.name));
        timestamp->tv_sec = other.timestamp->tv_sec;
        timestamp->tv_usec = other.timestamp->tv_usec;
    }

    TimerClass(TimerClass &&other) noexcept
        : timestamp{other.timestamp}, name{other.name} {
        other.timestamp = nullptr;
        other.name = nullptr;
    }

    TimerClass &operator=(const TimerClass &other) {
        if (this == &other) return *this;
        const auto newName = new char[strlen(other.name)];
        delete[] name;
        name = newName;
        std::strncpy(name, other.name, strlen(other.name));

        const auto newTimestamp = new timeval;
        delete timestamp;
        timestamp = newTimestamp;
        timestamp->tv_sec = other.timestamp->tv_sec;
        timestamp->tv_usec = other.timestamp->tv_usec;
        return *this;
    };

    TimerClass &operator=(TimerClass &&other) noexcept {
        if (this == &other) return *this;
        delete timestamp;
        delete[] name;
        timestamp = other.timestamp;
        name = other.name;
        other.timestamp = nullptr;
        other.name = nullptr;
        return *this;
    }

    ~TimerClass() {
        timeval *currTime = new timeval{};
        gettimeofday(currTime, NULL);
        if (!(timestamp == nullptr)) {
            if ((currTime->tv_sec - timestamp->tv_sec) == 0) {
                printf("%s age: %ld microseconds\n", name,
                       (currTime->tv_usec - timestamp->tv_usec));
            } else {
                printf(
                    "%s age: %ld seconds %ld microseconds\n", name,
                    (currTime->tv_sec - timestamp->tv_sec),
                    abs<__suseconds_t>(currTime->tv_usec - timestamp->tv_usec));
            }
        }
        delete timestamp;
        delete currTime;
        delete[] name;
    }

   private:
    timeval *timestamp;
    char *name;
};

int mathLoop(int end) {
    int result = 0;
    TimerClass mathLoopTimer{"MathLoopTimer"};
    for (auto i = 0; i < end; i++) {
        result += end * (end + 1) - (end % 2);
    }
    return result;
}

long fib(unsigned x) {
    if (x < 2) return x;
    return fib(x - 1) + fib(x - 2);
}

long fibHandler(unsigned x) {
    TimerClass fibon{"Fib"};
    return fib(x);
}

unsigned long long factorial(unsigned long long n) {
    // TimerClass factorialTimer{"FactorialTimer"};
    unsigned long long result = 0;
    if (n < 1) {
        result = 1;
    } else {
        result = n * factorial(n - 1);
    }
    return result;
}

unsigned long long factorialHandler(long long n) {
    TimerClass factorialTimer{"FactorialTimer"};
    return factorial(n);
}

unsigned long long exp(int a, int b) {
    TimerClass expTimer{"ExpTimer"};
    int result = a;
    for (auto i = 0; i < b; i++) {
        result *= b;
    }
    return result;
}

int main() {
    // TimerClass timer1{"timer1"};
    // TimerClass timer2{"timer2"};
    // timer2 = timer1;
    // TimerClass timer3{"timer3"};
    // timer3 = std::move(timer2);
    // TimerClass timer4{timer1};
    // mathLoop(500);
    // printf("%llu\n", factorialHandler(20));
    // int num = 0;
    // printf("Enter number\n");
    // std::cin >> num;
    // printf("fib(%d): %lu\n", num, fibHandler(num));

    int num = 0;
    printf("Enter a number:\n");
    std::cin >> num;
    TimerClass timer1{"fib"};
    TimerClass timer2{std::move(timer1)};
    printf("fib(%d): %lu\n", num, fib(num));

    return 0;
}
