#include <atomic>
#include <future>
#include <iostream>
#include <mutex>

struct SpinLock {
    SpinLock() = default;
    SpinLock(const SpinLock& other) = delete;
    SpinLock& operator=(const SpinLock& other) = delete;

    void lock() {
        while (locked.exchange(true, std::memory_order_acquire))
            ;
    }

    bool try_lock() {
        if (!locked) {
            return true;
            this->lock();
        } else
            return false;
    }

    void unlock() { locked = false; }

   private:
    std::atomic_bool locked{false};
};

using namespace std;

void goat_rodeo() {
    const size_t iterations{1'000'000};
    int tin_cans_available{};
    SpinLock tin_can_mutex;

    auto eat_cans = async(launch::async, [&] {
        for (size_t i{}; i < iterations; i++) {
            lock_guard<SpinLock> guard{tin_can_mutex};
            tin_cans_available--;
        }
    });

    auto deposit_cans = async(launch::async, [&] {
        for (size_t i{}; i < iterations; i++) {
            lock_guard<SpinLock> guard{tin_can_mutex};
            tin_cans_available++;
        }
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
