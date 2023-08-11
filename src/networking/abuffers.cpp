#include <boost/asio.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

namespace asio = boost::asio;
using std::cout, std::endl, std::string, std::cerr, std::vector;

/* Buffer types */

TEST_CASE("const_buffer default constructor") {
    asio::const_buffer cb;
    REQUIRE(cb.size() == 0);
}

TEST_CASE("mutable_buffer default constructor") {
    asio::mutable_buffer mb;
    REQUIRE(mb.size() == 0);
}

TEST_CASE("streambuf default constructor") {
    boost::asio::streambuf sb;
    REQUIRE(sb.size() == 0);
}

TEST_CASE("const_buffer constructor") {
    asio::const_buffer cb{"Blessed are the cheese makers", 7};

    REQUIRE(cb.size() == 7);
    REQUIRE(*static_cast<const char*>(cb.data()) == 'B');
}

TEST_CASE("mutable_buffer constructor") {
    string words{"SHADOW WIZARD MONEY GANG"};
    asio::mutable_buffer mb{words.data(), words.size()};

    REQUIRE(mb.data() == words.data());
    REQUIRE(mb.size() == words.size());
}

TEST_CASE("streambuf in/out") {
    asio::streambuf sb;
    std::ostream os{&sb};
    os << "Welease Wodger";

    std::istream is{&sb};
    string command;
    is >> command;

    REQUIRE(command == "Welease");
}

/* Buffer types */
