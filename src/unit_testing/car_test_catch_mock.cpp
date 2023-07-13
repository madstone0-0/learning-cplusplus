#include <catch2/catch.hpp>
#define CATCH_CONFIG_MAIN
#include "car.cpp"
#include "hippomocks.h"

using namespace Catch;

TEST_CASE("AutoBrake") {
    MockRepository mocks;
    mocks.autoExpect = false;

    auto* bus = mocks.Mock<IServiceBus>();
}
