#define BOOST_TEST_MODULE car_test_boost
#include <boost/test/included/unit_test.hpp>

#include "car.cpp"
#include "mock_og.cpp"

struct AutoBrakeTest {
    MockServiceBus bus{};
    AutoBrake brake{bus};
};

BOOST_FIXTURE_TEST_CASE(InitiatlCarSpeedIsZero, AutoBrakeTest) { BOOST_TEST(0 == brake.getSpeedMPS()); }

BOOST_FIXTURE_TEST_CASE(IntialSensitivityIsFive, AutoBrakeTest) { BOOST_TEST(5 == brake.getCollisionThresholdS()); }

BOOST_FIXTURE_TEST_CASE(SensitivityGreaterThanOne, AutoBrakeTest) {
    BOOST_REQUIRE_THROW(brake.setCollisionThresholdS(0.5L), std::exception);
}

BOOST_FIXTURE_TEST_CASE(SpeedIsSaved, AutoBrakeTest) {
    bus.speedUpdateCallback(SpeedUpdate{50L});
    BOOST_TEST(50 == brake.getSpeedMPS());
    bus.speedUpdateCallback(SpeedUpdate{100L});
    BOOST_TEST(100 == brake.getSpeedMPS());
    bus.speedUpdateCallback(SpeedUpdate{0L});
    BOOST_TEST(0 == brake.getSpeedMPS());
}

BOOST_FIXTURE_TEST_CASE(AlertWhenCrashImminent, AutoBrakeTest) {
    brake.setCollisionThresholdS(10L);
    bus.speedUpdateCallback(SpeedUpdate{100L});
    bus.carDetectedCallback(CarDetected{100L, 0L});
    BOOST_TEST(bus.lastCommand.time_to_collision_s, 1L);
    BOOST_TEST(bus.commandsPublished == 1);
}

BOOST_FIXTURE_TEST_CASE(NoAlertWhenNotImminent, AutoBrakeTest) {
    brake.setCollisionThresholdS(2L);
    bus.speedUpdateCallback(SpeedUpdate{100L});
    bus.carDetectedCallback(CarDetected{1000L, 50L});
    BOOST_TEST(bus.commandsPublished == 0);
}
