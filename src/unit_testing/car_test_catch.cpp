#include "catch_amalgamated.hpp"
#define CATCH_CONFIG_MAIN
#include <stdexcept>

#include "car.cpp"
#include "mock_og.cpp"

using namespace Catch;

TEST_CASE("AutoBrake") {
    MockServiceBus bus{};
    AutoBrake brake{bus};

    SECTION("Initializes speed to zero") { REQUIRE(brake.getSpeedMPS() == Approx(0)); }

    SECTION("Initializes sensitivity to five") { REQUIRE(brake.getCollisionThresholdS() == Approx(5)); }

    SECTION("Throws when sensitivity less than one") { REQUIRE_THROWS(brake.setCollisionThresholdS(0.5L)); }

    SECTION("Saves speed after update") {
        bus.speedUpdateCallback(SpeedUpdate{100L});
        REQUIRE(brake.getSpeedMPS() == 100L);
        bus.speedUpdateCallback(SpeedUpdate{50L});
        REQUIRE(brake.getSpeedMPS() == 50L);
        bus.speedUpdateCallback(SpeedUpdate{0L});
        REQUIRE(brake.getSpeedMPS() == 0L);
    }

    SECTION("Alerts when crash imminent") {
        brake.setCollisionThresholdS(10L);
        bus.speedUpdateCallback(SpeedUpdate{100L});
        bus.carDetectedCallback(CarDetected{100L, 0L});
        REQUIRE(bus.lastCommand.time_to_collision_s == 1);
        REQUIRE(bus.commandsPublished == Approx(1));
    }

    SECTION("No alert when crash not imminent") {
        brake.setCollisionThresholdS(2L);
        bus.speedUpdateCallback(SpeedUpdate{100L});
        bus.carDetectedCallback(CarDetected{1000L, 50L});
        REQUIRE(bus.commandsPublished == 0);
    }
}
