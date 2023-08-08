#include <catch2/catch_all.hpp>

#include "car.cpp"
#include "hippomocks.h"

using namespace Catch;

TEST_CASE("AutoBrake") {
    MockRepository mocks;
    mocks.autoExpect = false;

    auto* bus = mocks.Mock<IServiceBus>();
}
