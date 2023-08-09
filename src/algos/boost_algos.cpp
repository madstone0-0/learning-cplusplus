#include <boost/algorithm/searching/boyer_moore.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

using namespace boost::algorithm;
using namespace std;
using namespace string_literals;

TEST_CASE("boyer_moore_search") {
    string words{"Auntie Annes Alligator"};
    const auto res = boyer_moore_search(words.cbegin(), words.cend(), "Annes"s);

    REQUIRE(*(res.first) == *(words.begin() + 7));
}
