#include "timer.hpp"

using std::strncpy, std::cout, std::chrono::high_resolution_clock, std::chrono::time_point;

template <typename T>
constexpr T abs(T x) {
    return x < 0 ? x * -1 : x;
}

TimerClass::TimerClass(const char *x)
    : timestamp{new time_point<high_resolution_clock>{high_resolution_clock::now()}}, name{new char[strlen(x)]{}} {
    snprintf(name.get(), strlen(x), "%s", x);
}

TimerClass::TimerClass(const TimerClass &other)
    : timestamp{new time_point<high_resolution_clock>{other.timestamp.get()->time_since_epoch()}},
      name{new char[strlen(other.name.get())]{}} {
    strncpy(name.get(), other.name.get(), strlen(other.name.get()));
}

TimerClass::TimerClass(TimerClass &&other) noexcept
    : timestamp{std::move(other.timestamp)}, name{std::move(other.name)} {}

TimerClass &TimerClass::operator=(const TimerClass &other) {
    if (this == &other) return *this;
    timestamp = other.timestamp;

    name = other.name;

    return *this;
}

TimerClass &TimerClass::operator=(TimerClass &&other) noexcept {
    if (this == &other) return *this;

    name = std::move(other.name);
    timestamp = std::move(other.timestamp);

    return *this;
}

TimerClass::~TimerClass() {
    using namespace std::chrono;
    using namespace std::literals::chrono_literals;
    auto currTime = high_resolution_clock::now();

    if (timestamp != nullptr) {
        auto duration = currTime - *timestamp;
        if (duration_cast<seconds>(duration).count() <= 0) {
            cout << "(" << name.get() << ")"
                 << " age: " << duration_cast<microseconds>(duration) << "\n";

        } else {
            cout << "(" << name.get() << ")"
                 << " age: " << duration_cast<seconds>(duration) << "\n";
        }
    }
}
