#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <iostream>
#include <string>

/* Output */

#include <sstream>
TEST_CASE("ostringstream produces strings with str") {
    std::ostringstream ss;
    ss << "By Othin's beard, ";
    ss << "By the stump of Tyr. ";

    ss << "You shall be avenged";
    const auto thor = ss.str();

    ss.str("I am Groot");
    const auto groot = ss.str();

    REQUIRE(thor == "By Othin's beard, By the stump of Tyr. You shall be avenged");
    REQUIRE(groot == "I am Groot");
}

/* Output */

/* Input */

TEST_CASE("istringstream supports construction from a string") {
    std::string numbers{"1 2.23606 2"};
    std::istringstream ss{numbers};
    int a;
    float b, c, d;
    ss >> a;
    ss >> b;
    ss >> c;
    REQUIRE(a == 1);
    REQUIRE(b == Approx(2.23606));
    REQUIRE(c == Approx(2));
    REQUIRE_FALSE(ss >> d);
}

/* Input */

/* Both */

TEST_CASE("stringstream supports all strings stream ops") {
    std::stringstream ss;
    ss << "Zed's DEAD";

    std::string who;
    ss >> who;
    int what;
    ss >> std::hex >> what;

    REQUIRE(who == "Zed's");
    REQUIRE(what == 0xdead);
}

/* Both */
