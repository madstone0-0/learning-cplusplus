#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <regex>

/* Basic_Regex */

using std::string, std::regex, std::match_results, std::smatch;

// Matching
using std::regex_match;
TEST_CASE("std::basic_regex constructs from a string literal") {
    regex zipRegex{R"((\w{2})(\d{5})(-\d{4})?)"};
    REQUIRE(zipRegex.mark_count() == 3);
}

TEST_CASE("std::sub_match") {
    regex zipRegex{R"((\w{2})(\d{5})(-\d{4})?)"};
    smatch results;

    SECTION("returns true given a matching string") {
        string zip("NJ07936-3173");
        const auto matched = regex_match(zip, results, zipRegex);
        REQUIRE(matched);
        REQUIRE(results[0] == "NJ07936-3173");
        REQUIRE(results[1] == "NJ");
        REQUIRE(results[2] == "07936");
        REQUIRE(results[3] == "-3173");
    }

    SECTION("returns false given non-matching string") {
        string zip("Iomega Zip 100");
        const auto matched = regex_match(zip, results, zipRegex);
        REQUIRE_FALSE(matched);
    }
}

// Searching
using std::regex_search;
TEST_CASE("when only part of a string matches a regex") {
    regex zipRegex{R"((\w{2})(\d{5})(-\d{4})?)"};
    string sentence{"The string NJ07936-3173 is a ZIP Code."};

    SECTION("match returns false") { REQUIRE_FALSE(regex_match(sentence, zipRegex)); }

    SECTION("search returns true") { REQUIRE(regex_search(sentence, zipRegex)); }
}

// Replacing
using std::regex_replace;
TEST_CASE("std::regex_replace") {
    regex regex{"[aeiou]"};
    string phrase{"ueueing and cooeeing in eutopia"};
    const auto result = regex_replace(phrase, regex, "_");
    REQUIRE(result == "q_____ng _nd c_____ng _n __t_p__");
}

/* Basic_Regex */
