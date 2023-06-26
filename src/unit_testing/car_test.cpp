#include "car.cpp"

#include <stdexcept>

constexpr void assertThat(bool statment, const char* message = "Nope") {
    if (!statment) throw std::runtime_error{message};
}

void runTest(void (*unittest)(), const char* name) {
    try {
        unittest();
        printf("[+] Test [%s] successful\n", name);
    } catch (const std::exception& e) {
        printf("[-] Test failure in %s. %s.\n", name, e.what());
    }
}

void initialSpeedIs0() {
    MockServiceBus bus{};
    AutoBrake brake{bus};
    assertThat(brake.getSpeedMPS() == 0L, "Speed not equal to 0");
}

void initialSensIs5() {
    MockServiceBus bus{};
    AutoBrake brake{bus};
    assertThat(brake.getCollisionThresholdS() == 5L, "Sensitivity is not 5");
}

void sensGreaterThan1() {
    MockServiceBus bus{};
    AutoBrake brake{bus};
    try {
        brake.setCollisionThresholdS(0.5L);
    } catch (const std::exception&) {
        return;
    }
    assertThat(false, "no exception thrown");
}

void speedIsSaved() {
    MockServiceBus bus{};
    AutoBrake brake{bus};
    bus.speedUpdateCallback(SpeedUpdate{100L});
    assertThat(100L == brake.getSpeedMPS(), "Speed not saved to 100");
    bus.speedUpdateCallback(SpeedUpdate{50L});
    assertThat(50L == brake.getSpeedMPS(), "Speed not saved to 50");
    bus.speedUpdateCallback(SpeedUpdate{0L});
    assertThat(0L == brake.getSpeedMPS(), "Speed not saved to 0");
}

void alertWhenImminent() {
    MockServiceBus bus{};
    AutoBrake brake{bus};
    brake.setCollisionThresholdS(10L);
    bus.speedUpdateCallback(SpeedUpdate{100L});
    bus.carDetectedCallback(CarDetected{100L, 0L});
    assertThat(bus.lastCommand.time_to_collision_s == 1L, "time to collision not computed correctly");
    assertThat(bus.commandsPublished == 1, "brake commands published not one");
}

void noAlertWhenNotImminent() {
    MockServiceBus bus;
    AutoBrake brake{bus};
    brake.setCollisionThresholdS(2L);
    bus.speedUpdateCallback(SpeedUpdate{100L});
    bus.carDetectedCallback(CarDetected{1000L, 50L});
    assertThat(bus.commandsPublished == 0, "brake command published");
}

int main() {
    runTest(initialSpeedIs0, "initial speed is 0");
    runTest(initialSensIs5, "initial sensitivity is 5");
    runTest(sensGreaterThan1, "sensitivity greater than 1");
    runTest(speedIsSaved, "speed is saved on change");
    runTest(alertWhenImminent, "alerts when crash is imminent");
    runTest(noAlertWhenNotImminent, "no alert when not imminent");
    return 0;
}
