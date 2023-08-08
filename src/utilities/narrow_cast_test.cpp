
#include "narrow_cast.cpp"

#include <catch2/catch_all.hpp>
#include <limits>

TEST_CASE("narrow_cast returns normally") {
    auto val1 = 500L;
    auto res = narrow_cast<int>(val1);
    REQUIRE_FALSE(res == std::nullopt);
    REQUIRE(res == val1);
}

TEST_CASE("narrow_cast returns empty optional when narrowing occurs") {
    auto val1 = std::numeric_limits<long>::max();
    auto res = narrow_cast<int>(val1);
    REQUIRE(res == std::nullopt);
}
