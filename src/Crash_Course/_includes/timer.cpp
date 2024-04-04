#include "timer.hpp"

#include <chrono>

using namespace std::chrono_literals;
using std::cout, std::chrono::high_resolution_clock, std::chrono::time_point, std::chrono::duration;

namespace {
    template <typename T, typename U>
    std::string formatSeconds(const std::chrono::duration<T, U> &duration, const std::shared_ptr<std::string> &name) {
        std::stringstream ss;
        if (duration_cast<std::chrono::seconds>(duration).count() <= 0) {
            ss << "(" << *(name) << ")" << " age: " << duration_cast<std::chrono::microseconds>(duration) << "\n";

        } else {
            ss << "(" << *(name) << ")" << " age: " << duration_cast<std::chrono::seconds>(duration) << "\n";
        }
        return ss.str();
    }
}  // namespace

TimerClass::TimerClass(const std::string &x)
    : timestamp{new time_point<high_resolution_clock>{high_resolution_clock::now()}}, name{new std::string{x}} {}

TimerClass::TimerClass(const TimerClass &other)
    : timestamp{new time_point<high_resolution_clock>{other.timestamp->time_since_epoch()}},
      name{new std::string{*other.name}} {}

TimerClass::TimerClass(TimerClass &&other) noexcept
    : timestamp{std::move(other.timestamp)}, name{std::move(other.name)} {}

TimerClass::TimerClass(const std::string &x, std::chrono::nanoseconds *duration)
    : timestamp{new time_point<high_resolution_clock>{high_resolution_clock::now()}},
      name{new std::string{x}},
      duration{duration} {
    // this->duration.reset(duration);
}

TimerClass::TimerClass(const std::string &x, std::chrono::nanoseconds *duration, const bool &print)
    : timestamp{new time_point<high_resolution_clock>{high_resolution_clock::now()}},
      name{new std::string{x}},
      duration{duration},
      ptrPrint{print} {
    // this->duration.reset(duration);
}

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

    if (timestamp != nullptr) {
        if (duration != nullptr) {
            *duration = high_resolution_clock::now() - *timestamp;
            if (ptrPrint) cout << formatSeconds(*duration, name);
        } else {
            auto duration = high_resolution_clock::now() - *timestamp;
            cout << formatSeconds(duration, name);
        }
    }
}
