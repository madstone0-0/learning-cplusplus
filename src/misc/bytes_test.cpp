#define CATCH_CONFIG_MAIN
#include "bytes.hpp"

#include <catch2/catch.hpp>

TEST_CASE("ConvertBytes") {
    using namespace Bytes;
    using Catch::Detail::Approx;

    SECTION("Converts bytes to kilobytes") {
        auto result = convert(1024, "B", "KB");
        REQUIRE(result.value == Approx(1));
        REQUIRE(result.unit == "KB");
    }

    SECTION("Converts kilobytes to bytes") {
        auto result = convert(1, "KB", "B");
        REQUIRE(result.value == Approx(1024));
        REQUIRE(result.unit == "B");
    }

    SECTION("Converts megabytes to kilobytes") {
        auto result = convert(1, "MB", "KB");
        REQUIRE(result.value == Approx(1024));
        REQUIRE(result.unit == "KB");
    }

    SECTION("Converts kilobytes to megabytes") {
        auto result = convert(1, "KB", "MB");
        REQUIRE(result.value == Approx(1.0 / 1024));
        REQUIRE(result.unit == "MB");
    }

    SECTION("Converts gigabytes to megabytes") {
        auto result = convert(1, "GB", "MB");
        REQUIRE(result.value == Approx(1024));
        REQUIRE(result.unit == "MB");
    }

    SECTION("Converts megabytes to gigabytes") {
        auto result = convert(1, "MB", "GB");
        REQUIRE(result.value == Approx(1.0 / 1024));
        REQUIRE(result.unit == "GB");
    }

    SECTION("Converts terabytes to gigabytes") {
        auto result = convert(1, "TB", "GB");
        REQUIRE(result.value == Approx(1024));
        REQUIRE(result.unit == "GB");
    }

    SECTION("Converts gigabytes to terabytes") {
        auto result = convert(1, "GB", "TB");
        REQUIRE(result.value == Approx(1.0 / 1024));
        REQUIRE(result.unit == "TB");
    }

    SECTION("Converts petabytes to terabytes") {
        auto result = convert(1, "PB", "TB");
        REQUIRE(result.value == Approx(1024));
        REQUIRE(result.unit == "TB");
    }

    SECTION("Converts terabytes to petabytes") {
        auto result = convert(1, "TB", "PB");
        REQUIRE(result.value == Approx(1.0 / 1024));
        REQUIRE(result.unit == "PB");
    }

    SECTION("Same unit conversions return the same value") {
        const char* units[]{"B", "KB", "MB", "GB", "TB", "PB"};
        for (const auto& unit : units) {
            auto result = convert(1, unit, unit);
            REQUIRE(result.value == Approx(1));
            REQUIRE(result.unit == unit);
        }
    }
}
