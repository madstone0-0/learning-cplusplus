#include <catch2/catch_all.hpp>
#include <deque>
#include <forward_list>
#include <iterator>
#include <list>
#include <vector>

/* Input Iterators */

TEST_CASE("std::forward_list begin and end provide input iterators") {
    std::forward_list<int> easyAs{1, 2, 3};
    auto itr = easyAs.begin();

    REQUIRE(*itr == 1);
    itr++;
    REQUIRE(*itr == 2);
    itr++;
    REQUIRE(*itr == 3);
    itr++;
    REQUIRE(itr == easyAs.end());
}

/* Input Iterators */

/* Foward Iterator */

TEST_CASE("std::forward_list's begin and end provide forward iterators") {
    std::forward_list<int> easyAs{1, 2, 3};
    auto itr1 = easyAs.begin();
    auto itr2{itr1};
    int doubleSum{};
    while (itr1 != easyAs.end()) doubleSum += *(itr1++);
    while (itr2 != easyAs.end()) doubleSum += *(itr2++);
    REQUIRE(doubleSum == 12);
}

/* Foward Iterator */

/* Bidirectional Iterator */

TEST_CASE("std::list begin and end provide bidirectional iterators") {
    const std::list<int> easyAs{1, 2, 3};
    auto itr = easyAs.begin();
    REQUIRE(*itr == 1);
    itr++;
    REQUIRE(*itr == 2);
    itr--;
    REQUIRE(*itr == 1);
    REQUIRE(itr == easyAs.cbegin());
}

/* Bidirectional Iterator */

/* Random Access Iterator */

TEST_CASE("std::vector begin and end provide random-access iterators") {
    const std::vector<int> easyAs{1, 2, 3};
    auto itr = easyAs.begin();
    REQUIRE(itr[0] == 1);
    itr++;
    REQUIRE(*(easyAs.cbegin() + 2) == 3);
    REQUIRE(easyAs.cend() - itr == 2);
}

// Mutable Iterator
TEST_CASE("Mutable random-access iterators supports writing") {
    std::deque<int> easyAs{1, 2, 3};
    auto itr = easyAs.begin();
    itr[1] = 2;
    REQUIRE(*itr == 2);
}
/* Random Access Iterator */
