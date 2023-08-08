
#include <catch2/catch_all.hpp>
#include <memory>

#include "dead_men.cpp"

auto sp = std::make_shared<int>(10);
std::weak_ptr<int> wp{sp};

/* Weak Pointer */

auto& oaths = DeadMenOfDunharrow::oathsToFulfill;
TEST_CASE("Weak Pointer lock() yields") {
    auto message = "The way is shut.";
    SECTION("A shared pointer when tracked object is alive") {
        auto aragon = std::make_shared<DeadMenOfDunharrow>(message);
        std::weak_ptr<DeadMenOfDunharrow> legolas{aragon};
        auto shared = legolas.lock();
        REQUIRE(shared->message == message);
        REQUIRE(shared.use_count() == 2);
    }

    SECTION("Empty when shared pointer is empty") {
        std::weak_ptr<DeadMenOfDunharrow> legolas;
        {
            auto aragon = std::make_shared<DeadMenOfDunharrow>(message);
            legolas = aragon;
        }
        auto shared = legolas.lock();
        REQUIRE(shared == nullptr);
    }
}

/* Weak Pointer */
