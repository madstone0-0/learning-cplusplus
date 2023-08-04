#define CATCH_CONFIG_MAIN
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <catch2/catch.hpp>
#include <string>

/* Boost Range */

using std::string;
TEST_CASE("boost::algorithm") {
    using namespace boost::algorithm;
    using namespace std::string_literals;

    string word{"cymotrichous"};
    SECTION("start_with tests a string's beginning") { REQUIRE(starts_with(word, "cymo"s)); }

    SECTION("istarts_with is case insensitive") { REQUIRE(istarts_with(word, "cYmO"s)); }
}

TEST_CASE("boost::algorithm::all evaluates a predicate for all elements") {
    using namespace boost::algorithm;
    string word("juju");
    REQUIRE(all(word, [](auto c) { return c == 'j' || c == 'u'; }));
}

/* Boost Range */

/* Boost Classifiers */

TEST_CASE("boost::algorithm::is:alnum") {
    using namespace boost::algorithm;
    const auto classifier = is_alnum();
    SECTION("Evaluates alphanumeric characters") {
        REQUIRE(classifier('a'));
        REQUIRE_FALSE(classifier('$'));
    }

    SECTION("works with all") {
        REQUIRE(all("nostarch", classifier));
        REQUIRE_FALSE(all("@nostarch", classifier));
    }
}

/* Boost Classifiers */

/* Boost Finders */

#include <boost/algorithm/string/finder.hpp>

TEST_CASE("boost::algorithm::nth_finder finds the nth occurrence") {
    const auto finder = boost::algorithm::nth_finder("na", 1);
    string name{"Carl Brutananadilewsk"};

    const auto result = finder(name.begin(), name.end());
    REQUIRE(result.begin() == name.begin() + 12);  // Brutana(n)adilewski
    REQUIRE(result.end() == name.begin() + 14);    // Brutanana(d)ilewski
}

/* Boost Finders */

/* Modifying Algos */

// Case Convert

#include <boost/algorithm/string/case_conv.hpp>
TEST_CASE("boost::algorithm::to_upper") {
    string powers{"difficulty controlling the volume of my voice"};
    SECTION("upper-cases a string") {
        boost::algorithm::to_upper(powers);
        REQUIRE(powers == "DIFFICULTY CONTROLLING THE VOLUME OF MY VOICE");
    }

    SECTION("_copy leaves the original unmodified") {
        auto res = boost::algorithm::to_upper_copy(powers);
        REQUIRE(powers == "difficulty controlling the volume of my voice");
        REQUIRE(res == "DIFFICULTY CONTROLLING THE VOLUME OF MY VOICE");
    }
}

// Replace

#include <boost/algorithm/string/replace.hpp>
TEST_CASE("boost::algorithm::replace_first") {
    using namespace boost::algorithm;

    string publisher{"No Starch Press"};
    SECTION("replaces the first occurrence in a string") { replace_first(publisher, "No", "Medium"); }

    SECTION("has a case-insensitive variant") {
        auto res = ireplace_first_copy(publisher, "NO", "MEDIUM");
        REQUIRE(publisher == "No Starch Press");
        REQUIRE(res == "MEDIUM Starch Press");
    }
}

// Splitting and Joining

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
TEST_CASE("boost::algorithm::split splits a range based on a predicate") {
    using namespace boost::algorithm;
    string publisher{"No Starch Press"};
    std::vector<string> tokens;
    split(tokens, publisher, is_space());

    REQUIRE(tokens[0] == "No");
    REQUIRE(tokens[1] == "Starch");
    REQUIRE(tokens[2] == "Press");
}

#include <boost/algorithm/string/join.hpp>
TEST_CASE("boost::algorithm::join staples tokens together") {
    std::vector<string> tokens{"We invited the strippers", "JFK", "and Stalin"};
    auto res = boost::algorithm::join(tokens, ", ");
    REQUIRE(res == "We invited the strippers, JFK, and Stalin");
}

/* Modifying Algos */

/* Searching */

#include <boost/algorithm/string/find.hpp>
TEST_CASE("boost::algorithm::find_head computes the head") {
    string word{"blandishment"};
    const auto res = boost::algorithm::find_head(word, 5);
    REQUIRE(res.begin() == word.begin());    // (b)landishment
    REQUIRE(res.end() == word.begin() + 5);  // bland(i)shment
}

/* Searching */

/* Tokenizer */

#include <boost/tokenizer.hpp>
TEST_CASE("boost::tokenizer splits token-delimited strings") {
    string palindrome{"A man, a plan, a canal, Panama!"};
    boost::char_separator<char> comma(",");
    boost::tokenizer<boost::char_separator<char>> tokens{palindrome, comma};

    auto itr = tokens.begin();
    REQUIRE(*itr == "A man");
    itr++;
    REQUIRE(*itr == " a plan");
    itr++;
    REQUIRE(*itr == " a canal");
    itr++;
    REQUIRE(*itr == " Panama!");
}

/* Tokenizer */
