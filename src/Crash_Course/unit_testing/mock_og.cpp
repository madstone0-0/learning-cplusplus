#ifndef MOCK
#define MOCK

#include "iservice.cpp"

struct MockServiceBus : IServiceBus {
    void publish(const BrakeCommand& cmd) override {
        commandsPublished++;
        lastCommand = cmd;
    }

    void subscribe(SpeedUpdateCallback callback) override { speedUpdateCallback = callback; }

    void subscribe(CarDetectedCallback callback) override { carDetectedCallback = callback; }

    void subscribe(SpeedLimitDetectedCallback callback) override { speedLimitDetectedCallback = callback; }

    int commandsPublished{};
    BrakeCommand lastCommand;
    SpeedUpdateCallback speedUpdateCallback;
    CarDetectedCallback carDetectedCallback;
    SpeedLimitDetectedCallback speedLimitDetectedCallback;
};

#endif
