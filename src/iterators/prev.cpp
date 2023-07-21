#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <iterator>
#include <vector>

using std::prev;
TEST_CASE("prev returns iterators at given offsets") {
    std::vector<unsigned char> mission{0x9e, 0xc4, 0xc1, 0x29, 0x49, 0xa4, 0xf3, 0x14,
                                       0x74, 0xf2, 0x99, 0x05, 0x8c, 0xe2, 0xb2, 0x2a};

    auto itr1 = mission.begin();
    advance(itr1, 4);
    REQUIRE(*itr1 == 0x49);

    auto itr2 = prev(itr1, -1);
    REQUIRE(*itr2 == 0xa4);

    auto itr3 = prev(itr1, -4);
    REQUIRE(*itr3 == 0x74);

    REQUIRE(*itr1 == 0x49);
}
