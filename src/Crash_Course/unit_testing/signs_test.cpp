#include "signs.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "iservice.cpp"

struct MockServiceBus : IServiceBus {
    MOCK_METHOD1(publish, void(const BrakeCommand& cmd));
    MOCK_METHOD1(subscribe, void(const SpeedUpdateCallback callback));
    MOCK_METHOD1(subscribe, void(const CarDetectedCallback callback));
    MOCK_METHOD1(subscribe, void(const SpeedLimitDetectedCallback callback));
};

using ::testing::_;
using ::testing::A;
using ::testing::DoubleEq;
using ::testing::Field;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::StrictMock;

struct NiceAutoBrakeTest : ::testing::Test {
    NiceMock<MockServiceBus> bus;
    AutoBrake brake{bus};
};

struct StrictAutoBrakeTest : ::testing::Test {
    StrictAutoBrakeTest() {
        EXPECT_CALL(bus, subscribe(A<CarDetectedCallback>())).Times(1).WillOnce(Invoke([this](const auto& x) {
            carDetectedCallback = x;
        }));

        EXPECT_CALL(bus, subscribe(A<SpeedUpdateCallback>())).Times(1).WillOnce(Invoke([this](const auto& x) {
            speedUpdatedCallback = x;
        }));

        EXPECT_CALL(bus, subscribe(A<SpeedLimitDetectedCallback>())).Times(1).WillOnce(Invoke([this](const auto& x) {
            speedLimitDetectedCallback = x;
        }));
    }
    StrictMock<MockServiceBus> bus;
    CarDetectedCallback carDetectedCallback;
    SpeedUpdateCallback speedUpdatedCallback;
    SpeedLimitDetectedCallback speedLimitDetectedCallback;
};

TEST_F(NiceAutoBrakeTest, InitialCarSpeedIsZero) { ASSERT_DOUBLE_EQ(brake.getSpeedMPS(), 0); }

TEST_F(NiceAutoBrakeTest, IntialSensitivityIsFive) { ASSERT_DOUBLE_EQ(5, brake.getCollisionThresholdS()); }

TEST_F(NiceAutoBrakeTest, SensitivityGreaterThanOne) { ASSERT_ANY_THROW(brake.setCollisionThresholdS(0.5L)); }

TEST_F(NiceAutoBrakeTest, InitialLastSpeedis39) { ASSERT_EQ(brake.getLastSpeedLimitMPS(), 39); }

TEST_F(StrictAutoBrakeTest, SavesLastSpeed) {
    AutoBrake brake{bus};
    speedLimitDetectedCallback(SpeedLimitDetected{90L});
    speedLimitDetectedCallback(SpeedLimitDetected{40L});
    speedLimitDetectedCallback(SpeedLimitDetected{60L});

    ASSERT_EQ(brake.getLastSpeedLimitMPS(), 60L);
}

TEST_F(StrictAutoBrakeTest, NoAlertWhenUnderSpeedLimit) {
    AutoBrake brake{bus};
    speedLimitDetectedCallback(SpeedLimitDetected{35L});
    speedUpdatedCallback(SpeedUpdate{34L});
}

TEST_F(StrictAutoBrakeTest, AlertWhenOverSpeedLimit) {
    EXPECT_CALL(bus, publish(Field(&BrakeCommand::time_to_collision_s, DoubleEq(0L)))).Times(1);

    AutoBrake brake{bus};
    speedLimitDetectedCallback(SpeedLimitDetected{35L});
    speedUpdatedCallback(SpeedUpdate{40L});
}

TEST_F(StrictAutoBrakeTest, AlertWhenNewSpeedLimitIsLess) {
    EXPECT_CALL(bus, publish(Field(&BrakeCommand::time_to_collision_s, DoubleEq(0L)))).Times(1);

    AutoBrake brake{bus};
    speedLimitDetectedCallback(SpeedLimitDetected{35L});
    speedUpdatedCallback(SpeedUpdate{30L});
    speedLimitDetectedCallback(SpeedLimitDetected{25L});
}

TEST_F(StrictAutoBrakeTest, AlertWhenCrashImminent) {
    EXPECT_CALL(bus, publish(Field(&BrakeCommand::time_to_collision_s, DoubleEq(1L)))).Times(1);

    AutoBrake brake{bus};
    brake.setCollisionThresholdS(10L);
    speedLimitDetectedCallback(SpeedLimitDetected{150});
    speedUpdatedCallback(SpeedUpdate{100L});
    carDetectedCallback(CarDetected{100L, 0L});
}

TEST_F(StrictAutoBrakeTest, NoAlertWhenNotImminent) {
    AutoBrake brake{bus};

    brake.setCollisionThresholdS(2L);
    speedLimitDetectedCallback(SpeedLimitDetected{150});
    speedUpdatedCallback(SpeedUpdate{100L});
    carDetectedCallback(CarDetected{1000L, 50L});
}
