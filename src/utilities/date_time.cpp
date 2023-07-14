#define CATCH_CONFIG_MAIN
#include <boost/date_time/gregorian/gregorian.hpp>
#include <catch2/catch.hpp>
#include <chrono>
#include <iostream>
#include <thread>

#include "timer.hpp"

using std::cout;

/* Date */
boost::gregorian::date d{1982, 9, 15};

TEST_CASE("Invalid boost::Gregorian::dates throw exceptions") {
    using boost::gregorian::bad_day_of_month;
    using boost::gregorian::date;

    REQUIRE_THROWS_AS(date(2004, 01, 33), bad_day_of_month);

    auto dLocal = boost::gregorian::day_clock::local_day();
    auto dUniv = boost::gregorian::day_clock::universal_day();
    cout << dLocal << "\n" << dUniv << "\n";
}

TEST_CASE("boost::gregorian::date supports basic calendar functions") {
    boost::gregorian::date d{1986, 9, 15};
    REQUIRE(d.year() == 1986);
    REQUIRE(d.month() == 9);
    REQUIRE(d.day() == 15);
    REQUIRE(d.day_of_year() == 258);
    REQUIRE(d.day_of_week() == boost::date_time::Monday);
}

TEST_CASE("boost::gregorian::date supports calendar arithmetic") {
    boost::gregorian::date d1{2004, 1, 18};
    auto d2 = boost::gregorian::date{2023, 7, 12};

    auto duration = d2 - d1;
    REQUIRE(duration.days() == 7115);
}

TEST_CASE("date and date_duration support addition") {
    boost::gregorian::date d1{2004, 1, 18};
    boost::gregorian::date_duration dur{7115};
    auto d2 = d1 + dur;
    REQUIRE(d2 == boost::gregorian::from_string("2023/7/12"));
}

// Date Period

TEST_CASE("boost::gregorian::date supports periods+") {
    boost::gregorian::date d1{1986, 9, 15};
    boost::gregorian::date d2{2019, 8, 1};
    boost::gregorian::date_period p{d1, d2};
    REQUIRE(p.contains(boost::gregorian::date{1987, 10, 27}));
}

/* Date */

/* Chrono */

// Durations

TEST_CASE("std::chrono supports several clocks") {
    auto sysNow = std::chrono::system_clock::now();
    auto hiResNow = std::chrono::high_resolution_clock::now();
    auto steadyNow = std::chrono::steady_clock::now();

    REQUIRE(sysNow.time_since_epoch().count() > 0);
    REQUIRE(hiResNow.time_since_epoch().count() > 0);
    REQUIRE(steadyNow.time_since_epoch().count() > 0);
}

TEST_CASE("std::chrono supports several units of measurement") {
    using namespace std::literals::chrono_literals;

    auto oneS = std::chrono::seconds(1);
    auto thousandMS = 1000ms;
    REQUIRE(oneS == thousandMS);
}

TEST_CASE("std::chrono supports duration casts") {
    using namespace std::chrono;
    auto billionNSAsS = duration_cast<seconds>(1'000'000'000ns);
    REQUIRE(billionNSAsS.count() == 1);
}

// Waiting

TEST_CASE("std::chrono used to sleep") {
    using namespace std::literals::chrono_literals;

    auto start = std::chrono::system_clock::now();
    std::this_thread::sleep_for(100ms);
    auto end = std::chrono::system_clock::now();
    REQUIRE(end - start >= 100ms);
}

// Timing

TEST_CASE("Fuck it my timer") {
    const size_t n = 1'000'000;
    {
        TimerClass divs{"Divs"};
        volatile double result{1.23e45};
        for (double i = 1; i < n; i++) {
            result /= i;
        }
    }
    printf("Done :)\n");
}

/* Chrono */
