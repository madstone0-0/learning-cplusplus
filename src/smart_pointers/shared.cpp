
#include <catch2/catch_all.hpp>
#include <memory>

#include "dead_men.cpp"

// Only allocated owned dynamic object
std::shared_ptr<int> ptr{new int{808}};
// Allocates control block and owned dynamic object simultaneously
auto otherPtr = std::make_shared<int>(808);

std::shared_ptr<int> verbose{new int{808}, [](int* x) { delete x; }, std::allocator<int>{}};
auto customAllocatorMakeShared = std::allocate_shared<int>(std::allocator<int>{}, 10);

using SharedOathBreakers = std::shared_ptr<DeadMenOfDunharrow>;
using std::make_shared;

auto& oaths = DeadMenOfDunharrow::oathsToFulfill;
/* Shared Pointer */
TEST_CASE("Shared Pointer can be used in copy") {
    auto aragon = make_shared<DeadMenOfDunharrow>();
    SECTION("Construction") {
        auto sonOfAragon{aragon};
        REQUIRE(oaths == 1);
    }

    SECTION("Assignment") {
        auto sonOfAragon = aragon;
        REQUIRE(oaths == 1);
    }

    SECTION("Assignment and original gets discarded") {
        auto sonOfAragon = make_shared<DeadMenOfDunharrow>();
        REQUIRE(oaths == 2);
        sonOfAragon = aragon;
        REQUIRE(oaths == 1);
    }
}
/* Shared Pointer */

/* Shared Arrays*/

TEST_CASE("Shared Pointer to array supports operator[]") {
    std::shared_ptr<int[]> squares{new int[5]{1, 4, 9, 16, 25}};
    squares[0] = 1;

    REQUIRE(squares[0] == 1);
    REQUIRE(squares[1] == 4);
    REQUIRE(squares[2] == 9);
}

/* Shared Arrays*/
