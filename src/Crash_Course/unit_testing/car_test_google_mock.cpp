#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "car.cpp"
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
    }
    StrictMock<MockServiceBus> bus;
    CarDetectedCallback carDetectedCallback;
    SpeedUpdateCallback speedUpdatedCallback;
};

TEST_F(NiceAutoBrakeTest, InitialCarSpeedIsZero) { ASSERT_DOUBLE_EQ(brake.getSpeedMPS(), 0); }

TEST_F(NiceAutoBrakeTest, IntialSensitivityIsFive) { ASSERT_DOUBLE_EQ(5, brake.getCollisionThresholdS()); }

TEST_F(NiceAutoBrakeTest, SensitivityGreaterThanOne) { ASSERT_ANY_THROW(brake.setCollisionThresholdS(0.5L)); }

TEST_F(StrictAutoBrakeTest, AlertWhenCrashImminent) {
    EXPECT_CALL(bus, publish(Field(&BrakeCommand::time_to_collision_s, DoubleEq(1L)))).Times(1);

    AutoBrake brake{bus};
    brake.setCollisionThresholdS(10L);
    speedUpdatedCallback(SpeedUpdate{100L});
    carDetectedCallback(CarDetected{100L, 0L});
}

TEST_F(StrictAutoBrakeTest, NoAlertWhenNotImminent) {
    AutoBrake brake{bus};

    brake.setCollisionThresholdS(2L);
    speedUpdatedCallback(SpeedUpdate{100L});
    carDetectedCallback(CarDetected{1000L, 50L});
}
