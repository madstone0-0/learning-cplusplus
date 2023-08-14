#include "timer.hpp"

using namespace std::chrono_literals;
using std::cout, std::chrono::high_resolution_clock, std::chrono::time_point, std::chrono::duration;

TimerClass::TimerClass(const std::string &x)
    : timestamp{new time_point<high_resolution_clock>{high_resolution_clock::now()}}, name{new std::string{}} {
    name->append(x.c_str());
}

TimerClass::TimerClass(const TimerClass &other)
    : timestamp{new time_point<high_resolution_clock>{other.timestamp.get()->time_since_epoch()}},
      name{new std::string{}} {
    name->append(other.name->c_str());
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
            cout << "(" << *(name.get()) << ")"
                 << " age: " << duration_cast<microseconds>(duration) << "\n";

        } else {
            cout << "(" << *(name.get()) << ")"
                 << " age: " << duration_cast<seconds>(duration) << "\n";
        }
    }
}
