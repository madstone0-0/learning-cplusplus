#include <catch2/catch_test_macros.hpp>
#include <future>
#include <string>

using namespace std;

/* Futures */

TEST_CASE("async returns a valid future") {
    using namespace string_literals;
    auto the_future = async([] { return "man"s; });
    REQUIRE(the_future.valid());
}

TEST_CASE("future is invalid by default") {
    future<bool> default_future{};

    REQUIRE_FALSE(default_future.valid());
}

TEST_CASE("async returns the return value of the function object") {
    using namespace string_literals;
    auto the_future = async([] { return "man"s; });
    REQUIRE(the_future.get() == "man");
}

TEST_CASE("get may throw") {
    auto ghostrider = async([] { throw runtime_error("The pattern is full."); });
    REQUIRE_THROWS_AS(ghostrider.get(), runtime_error);
}

TEST_CASE("wait_for indicates whether a task is ready") {
    using namespace chrono_literals;
    auto sleepy = async([] { this_thread::sleep_for(100ms); });
    const auto not_ready_yet = sleepy.wait_for(25ms);
    REQUIRE(not_ready_yet == future_status::timeout);

    const auto ready = sleepy.wait_for(100ms);
    REQUIRE(ready == future_status::ready);
}

/* Futures */
