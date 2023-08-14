
#include <catch2/catch_all.hpp>
#include <memory>
#include <utility>

#include "dead_men.cpp"

std::unique_ptr<int> ptr{new int{808}};
auto otherPtr = std::make_unique<int>(808);

using UniqueOathBreaers = std::unique_ptr<DeadMenOfDunharrow>;

auto& oaths = DeadMenOfDunharrow::oathsToFulfill;
/* Unique Pointer */
TEST_CASE("Unique Pointer can be used in move") {
    auto aragon = UniqueOathBreaers{new DeadMenOfDunharrow{}};

    SECTION("construction") {
        auto sonOfAragon{std::move(aragon)};
        REQUIRE(oaths == 1);
    }

    SECTION("assignment") {
        auto sonOfAragon = UniqueOathBreaers{new DeadMenOfDunharrow};
        REQUIRE(oaths == 2);
        sonOfAragon = std::move(aragon);
        REQUIRE(oaths == 1);
    }
}
/* Unique Pointer */

/*Unique Array*/

TEST_CASE("Unique Pointer to array supports operator[]") {
    std::unique_ptr<int[]> squares{new int[5]{1, 4, 9, 16, 25}};
    squares[0] = 1;

    REQUIRE(squares[0] == 1);
    REQUIRE(squares[1] == 4);
    REQUIRE(squares[2] == 9);
}

/*Unique Array*/

/* Deleters */

auto deleter = [](int* x) {
    printf("Deleting an int %p.\n", x);
    delete x;
};

std::unique_ptr<int, decltype(deleter)> up{new int{4}, deleter};

/* Deleters */
