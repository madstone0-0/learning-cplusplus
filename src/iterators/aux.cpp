
#include <catch2/catch_all.hpp>
#include <iterator>
#include <list>

/* std::Advance */

using std::advance, std::next;
TEST_CASE("advance modifies input iterators") {
    std::vector<unsigned char> mission{0x9e, 0xc4, 0xc1, 0x29, 0x49, 0xa4, 0xf3, 0x14,
                                       0x74, 0xf2, 0x99, 0x05, 0x8c, 0xe2, 0xb2, 0x2a};

    auto itr = mission.begin();
    advance(itr, 4);
    REQUIRE(*itr == 0x49);

    std::advance(itr, 4);
    REQUIRE(*itr == 0x74);

    std::advance(itr, -8);
    REQUIRE(*itr == 0x9e);
}

/* std::Advance */

/* std::Next */

TEST_CASE("next returns iterators at given offsets") {
    std::vector<unsigned char> mission{0x9e, 0xc4, 0xc1, 0x29, 0x49, 0xa4, 0xf3, 0x14,
                                       0x74, 0xf2, 0x99, 0x05, 0x8c, 0xe2, 0xb2, 0x2a};

    auto itr1 = mission.begin();
    advance(itr1, 4);
    REQUIRE(*itr1 == 0x49);

    auto itr2 = next(itr1);
    REQUIRE(*itr2 == 0xa4);

    auto itr3 = next(itr1, 4);
    REQUIRE(*itr3 == 0x74);

    REQUIRE(*itr1 == 0x49);
}

/* std::Next */

/* std::Distance */

TEST_CASE("distance returns the number of elements between iterators") {
    std::vector<unsigned char> mission{0x9e, 0xc4, 0xc1, 0x29, 0x49, 0xa4, 0xf3, 0x14,
                                       0x74, 0xf2, 0x99, 0x05, 0x8c, 0xe2, 0xb2, 0x2a};

    auto eighth = next(mission.begin(), 8);
    auto fifth = std::prev(eighth, 3);
    REQUIRE(std::distance(fifth, eighth) == 3);
}

/* std::Distance */

/* std::Iter_Swap */

TEST_CASE("iter_swap swaps pointed to elements") {
    std::vector<long> easyAs{3, 2, 1};
    std::iter_swap(easyAs.begin(), next(easyAs.begin(), 2));
    REQUIRE(easyAs[0] == 1);
    REQUIRE(easyAs[1] == 2);
    REQUIRE(easyAs[2] == 3);
}

/* std::Iter_Swap */

/* std::Make_Move_Iterator */

struct Moveable {
    explicit Moveable(int id) : id{id} {}
    Moveable(Moveable&& moveable) {
        id = moveable.id;
        moveable.id = -1;
    }
    int id;
};

TEST_CASE("move iterators convert accesses into move operations") {
    std::vector<Moveable> donor;
    donor.emplace_back(1);
    donor.emplace_back(2);
    donor.emplace_back(3);

    std::vector<Moveable> recipient(std::make_move_iterator(donor.begin()), std::make_move_iterator(donor.end()));

    REQUIRE(donor[0].id == -1);
    REQUIRE(donor[1].id == -1);
    REQUIRE(donor[2].id == -1);

    REQUIRE(recipient[0].id == 1);
    REQUIRE(recipient[1].id == 2);
    REQUIRE(recipient[2].id == 3);
}

/* std::Make_Move_Iterator */

/* Reverse Iterator Adapter */

TEST_CASE("reverse iterators can initialize containers") {
    std::list<int> org{3, 2, 1};
    std::vector<int> easyAs(org.crbegin(), org.crend());

    REQUIRE(easyAs[0] == 1);
    REQUIRE(easyAs[1] == 2);
    REQUIRE(easyAs[2] == 3);
}

TEST_CASE("std::make_reverse_iterator converts a normal iterator") {
    std::list<int> org{3, 2, 1};
    auto begin = std::make_reverse_iterator(org.cend());
    auto end = std::make_reverse_iterator(org.cbegin());

    std::vector<int> easyAs(begin, end);
    REQUIRE(easyAs[0] == 1);
    REQUIRE(easyAs[1] == 2);
    REQUIRE(easyAs[2] == 3);
}

/* Reverse Iterator Adapter */
