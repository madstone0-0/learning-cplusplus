
#include <catch2/catch_all.hpp>
#include <deque>
#include <iterator>

/* Insert */

using std::back_inserter, std::front_inserter, std::inserter;
TEST_CASE("Insert iterators convert writes into container insertions") {
    std::deque<int> dq;
    auto backInsert = back_inserter(dq);
    *backInsert = 2;
    backInsert++;
    *backInsert = 4;
    backInsert++;

    auto frontInsert = front_inserter(dq);
    *frontInsert = 1;
    frontInsert++;

    auto insert = inserter(dq, dq.begin() + 2);
    *insert = 3;
    insert++;

    REQUIRE(dq[0] == 1);
    REQUIRE(dq[1] == 2);
    REQUIRE(dq[2] == 3);
    REQUIRE(dq[3] == 4);
}

/* Insert */
