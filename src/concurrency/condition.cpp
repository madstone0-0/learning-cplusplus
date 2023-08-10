#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>

using namespace std;

void goat_rodeo() {
    const size_t iterations{1'000'000};
    int tin_cans_available{};
    mutex m;
    condition_variable cv;

    auto eat_cans = async(launch::async, [&] {
        unique_lock<mutex> guard{m};
        cv.wait(guard, [&] { return tin_cans_available == 1'000'000; });
        for (size_t i{}; i < iterations; i++) tin_cans_available--;
    });

    auto deposit_cans = async(launch::async, [&] {
        scoped_lock<mutex> guard{m};
        for (size_t i{}; i < iterations; i++) tin_cans_available++;
        cv.notify_all();
    });

    eat_cans.get();
    deposit_cans.get();

    cout << "Tin cans: " << tin_cans_available << "\n";
}

int main() {
    goat_rodeo();
    goat_rodeo();
    goat_rodeo();
}
