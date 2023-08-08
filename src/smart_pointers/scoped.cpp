
#include <boost/smart_ptr/scoped_array.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <catch2/catch_all.hpp>

#include "dead_men.cpp"

/* Scoped Pointer */

using boost::scoped_ptr;

using ScopedOathBreakers = scoped_ptr<DeadMenOfDunharrow>;

TEST_CASE("Scoped Pointer evaluates to") {
    SECTION("true when full") {
        ScopedOathBreakers aragon{new DeadMenOfDunharrow{}};
        REQUIRE(aragon);
    }

    SECTION("false when empty") {
        ScopedOathBreakers aragon;
        REQUIRE_FALSE(aragon);
    }
}

auto& oathsToFulfill = DeadMenOfDunharrow::oathsToFulfill;
TEST_CASE("Scoped Pointer is a RAII wrapper") {
    REQUIRE(oathsToFulfill == 0);
    ScopedOathBreakers aragon{new DeadMenOfDunharrow{}};
    REQUIRE(oathsToFulfill == 1);
    {
        ScopedOathBreakers legolas{new DeadMenOfDunharrow{}};
        REQUIRE(oathsToFulfill == 2);
    }
    REQUIRE(oathsToFulfill == 1);
}

TEST_CASE("Scoped Pointer supports pointer semantics") {
    auto message = "The way is shut";

    ScopedOathBreakers aragon{new DeadMenOfDunharrow{message}};

    SECTION("operator*") { REQUIRE((*aragon).message == message); }

    SECTION("operator->") { REQUIRE(aragon->message == message); }

    SECTION("get() which returns a raw pointer") { REQUIRE(aragon.get() != nullptr); }
}

TEST_CASE("ScopedPtr supports comparison with nullptr") {
    SECTION("operator==") {
        ScopedOathBreakers legolas{};
        REQUIRE(legolas == nullptr);
    }
    SECTION("operator!=") {
        ScopedOathBreakers aragorn{new DeadMenOfDunharrow{}};
        REQUIRE(aragorn != nullptr);
    }
}

TEST_CASE("Scoped Pointer supports swap") {
    auto m1 = "The way is shut";
    auto m2 = "Until the time comes";

    ScopedOathBreakers aragon{new DeadMenOfDunharrow{m1}};
    ScopedOathBreakers legolas{new DeadMenOfDunharrow{m2}};

    aragon.swap(legolas);
    REQUIRE(legolas->message == m1);
    REQUIRE(aragon->message == m2);
}

TEST_CASE("Scoped Pointer reset") {
    ScopedOathBreakers aragon{new DeadMenOfDunharrow{}};
    SECTION("Destructs owned object") {
        aragon.reset();
        REQUIRE(oathsToFulfill == 0);
    }

    SECTION("Can replace owned object") {
        auto m = "It was made by those who are dead";
        auto newDeadMen = new DeadMenOfDunharrow{m};
        REQUIRE(oathsToFulfill == 2);
        aragon.reset(newDeadMen);
        REQUIRE(oathsToFulfill == 1);
        REQUIRE(aragon->message == newDeadMen->message);
        REQUIRE(aragon.get() == newDeadMen);
    }
}

void by_ref(const ScopedOathBreakers&) {}
void by_val(ScopedOathBreakers) {}

TEST_CASE("ScopedPtr can") {
    ScopedOathBreakers aragorn{new DeadMenOfDunharrow};
    SECTION("be passed by reference") { by_ref(aragorn); }
    SECTION("not be copied") {
        // DOES NOT COMPILE:
        // by_val(aragorn);
        // auto son_of_arathorn = aragorn;
    }
    SECTION("not be moved") {
        // DOES NOT COMPILE:
        // by_val(std::move(aragorn));
        // auto son_of_arathorn = std::move(aragorn);
    }
}

/* Scoped Pointer */

/* Scoped array */

TEST_CASE("Scoped array supports operator[]") {
    boost::scoped_array<int> squares{new int[]{0, 4, 9, 16, 25}};
    squares[0] = 1;
    REQUIRE(squares[0] == 1);
    REQUIRE(squares[1] == 4);
    REQUIRE(squares[2] == 9);
    REQUIRE(squares[3] == 16);
}

/* Scoped array */
