#include <fmt/core.h>

#include <cstdio>
#include <thread>

/* basics */

void doWork() {
    size_t i{};
    while (i < 1'000'0000) i++;
}

int main() {
    std::thread t{doWork};
    t.detach();
    return 0;
}

/* basics */
