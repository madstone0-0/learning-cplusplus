#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>

namespace ranges = std::ranges;

using std::string, std::vector, ranges::all_of, ranges::for_each, ranges::range, ranges::transform, std::min;

/* Determines whether each element in a
 * sequence meets some user specified criteria
 */
TEST_CASE("ranges all_of") {
    vector<string> words{"Auntie", "Annes", "Alligator"};
    const auto starts_with_a = [](const auto& word) {
        if (word.empty()) return false;
        return word[0] == 'a' || word[0] == 'A';
    };

    REQUIRE(all_of(words, starts_with_a));

    const auto has_length_six = [](const auto& word) { return word.length() == 6; };
    REQUIRE_FALSE(all_of(words, has_length_six));
}

/* Applies a function to each element in a
 * sequence
 */
TEST_CASE("for_each") {
    vector<string> words{"David", "Donald", "Doo"};
    size_t number_of_Ds{};

    const auto count_number_of_ds = [&number_of_Ds](const auto& word) {
        if (word.empty()) return;
        if (word[0] == 'D') number_of_Ds++;
    };

    for_each(words, count_number_of_ds);
    REQUIRE(number_of_Ds == 3);
}

/* Modifies the elements of one sequence
 * and writes them into another
 */
#include <boost/algorithm/string/case_conv.hpp>
TEST_CASE("transform") {
    vector<string> words1{"farewell", "hello", "farewell", "hello"};
    vector<string> res1;

    const auto upper = [](string word) {
        boost::to_upper(word);
        return word;
    };
    transform(words1, back_inserter(res1), upper);
    REQUIRE(res1 == vector<string>{"FAREWELL", "HELLO", "FAREWELL", "HELLO"});

    vector<string> words2{"light", "human", "bro", "quantum"};
    vector<string> words3{"radar", "robot", "pony", "bit"};
    vector<string> res2;

    auto portmantize = [](const auto& word1, const auto& word2) {
        const auto word1_letters = min(size_t{2}, word1.size());
        string res{word1.begin(), word1.begin() + word1_letters};

        const auto word2_letters = min(size_t{3}, word2.size());
        res.insert(res.end(), word2.end() - word2_letters, word2.end());
        return res;
    };

    transform(words2, words3, back_inserter(res2), portmantize);
    REQUIRE(res2 == vector<string>{"lidar", "hubot", "brony", "qubit"});
}
