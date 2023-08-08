
#include "timer.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Timer") {
    auto timer1 = TimerClass{"Timer1"};
    auto timer2 = TimerClass{"Timer2"};
    auto timer3 = TimerClass{"Timer3"};
    auto timer4 = TimerClass{"Timer4"};

    SECTION("Copy Constructor") {
        TimerClass test1{timer1};
        // REQUIRE(test1.name == timer1.name);
        REQUIRE(test1.timestamp->time_since_epoch() == timer1.timestamp->time_since_epoch());

        TimerClass test2{timer2};
        REQUIRE(test2.timestamp->time_since_epoch() == timer2.timestamp->time_since_epoch());
    }

    SECTION("Copy assignment") {
        auto name = "test1";
        TimerClass test1{name};

        // REQUIRE(test1.name.get() == name);

        test1 = timer3;
        REQUIRE(test1.name == timer3.name);
        REQUIRE(test1.timestamp->time_since_epoch() == timer3.timestamp->time_since_epoch());
    }

    SECTION("Move Constructor") {
        TimerClass test1{std::move(timer4)};
        auto name = "Timer4";
        // REQUIRE(test1.name.get() == name);

        REQUIRE(timer4.name.get() == nullptr);
        REQUIRE(timer4.timestamp.get() == nullptr);
    }

    SECTION("Move assignment") {
        TimerClass test1{"test1"};
        test1 = std::move(timer2);
        auto name = "Timer2";

        // REQUIRE(test1.name.get() == name);

        REQUIRE(timer2.name.get() == nullptr);
        REQUIRE(timer2.timestamp.get() == nullptr);
    }
}
