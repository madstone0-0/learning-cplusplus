#include <gtest/gtest.h>

#include "car.cpp"
#include "mock_og.cpp"

struct AutoBrakeTest : ::testing::Test {
    MockServiceBus bus{};
    AutoBrake brake{bus};
};

TEST_F(AutoBrakeTest, InitialCarSpeedIsZero) { ASSERT_DOUBLE_EQ(0, brake.getSpeedMPS()); };

TEST_F(AutoBrakeTest, IntialSensitivityIsFive) { ASSERT_DOUBLE_EQ(5, brake.getCollisionThresholdS()); }

TEST_F(AutoBrakeTest, SensitivityGreaterThanOne) { ASSERT_ANY_THROW(brake.setCollisionThresholdS(0.5L)); }

TEST_F(AutoBrakeTest, SpeedIsSaved) {
    bus.speedUpdateCallback(SpeedUpdate{50L});
    ASSERT_EQ(50, brake.getSpeedMPS());
    bus.speedUpdateCallback(SpeedUpdate{100L});
    ASSERT_EQ(100, brake.getSpeedMPS());
    bus.speedUpdateCallback(SpeedUpdate{0L});
    ASSERT_EQ(0, brake.getSpeedMPS());
}

TEST_F(AutoBrakeTest, AlertWhenCrashImminent) {
    brake.setCollisionThresholdS(10L);
    bus.speedUpdateCallback(SpeedUpdate{100L});
    bus.carDetectedCallback(CarDetected{100L, 0L});
    ASSERT_DOUBLE_EQ(bus.lastCommand.time_to_collision_s, 1L);
    ASSERT_EQ(bus.commandsPublished, 1);
}

TEST_F(AutoBrakeTest, NoAlertWhenNotImminent) {
    brake.setCollisionThresholdS(2L);
    bus.speedUpdateCallback(SpeedUpdate{100L});
    bus.carDetectedCallback(CarDetected{1000L, 50L});
    ASSERT_EQ(bus.commandsPublished, 0);
}
