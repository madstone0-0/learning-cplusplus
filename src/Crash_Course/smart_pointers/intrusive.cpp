
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <catch2/catch_all.hpp>
#include <memory>

#include "dead_men.cpp"

using IntrusiveOathBreakers = boost::intrusive_ptr<DeadMenOfDunharrow>;
size_t refCount{};

void intrusive_ptr_add_ref(DeadMenOfDunharrow* d) { refCount++; }

void intrusive_ptr_release(DeadMenOfDunharrow* d) {
    refCount--;
    if (refCount == 0) delete d;
}

auto& oaths = DeadMenOfDunharrow::oathsToFulfill;
TEST_CASE("Instrusive Pointer uses an embedded reference counter") {
    REQUIRE(refCount == 0);
    IntrusiveOathBreakers aragon{new DeadMenOfDunharrow{}};
    REQUIRE(refCount == 1);
    {
        IntrusiveOathBreakers legolas{aragon};
        REQUIRE(refCount == 2);
    }
    REQUIRE(oaths == 1);
}
