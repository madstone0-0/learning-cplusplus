#include <functional>
#include <stdexcept>

struct SpeedUpdate {
    double velocity_mps;
};

struct CarDetected {
    double distance_m;
    double velocity_mps;
};

struct BrakeCommand {
    double time_to_collision_s;
};

using SpeedUpdateCallback = std::function<void(const SpeedUpdate&)>;
using CarDetectedCallback = std::function<void(const CarDetected&)>;

struct IServiceBus {
    virtual ~IServiceBus() = default;
    virtual void publish(const BrakeCommand&) = 0;
    virtual void subscribe(SpeedUpdateCallback) = 0;
    virtual void subscribe(CarDetectedCallback) = 0;
};

struct MockServiceBus : IServiceBus {
    void publish(const BrakeCommand& cmd) override {
        commandsPublished++;
        lastCommand = cmd;
    }

    void subscribe(SpeedUpdateCallback callback) override { speedUpdateCallback = callback; }

    void subscribe(CarDetectedCallback callback) override { carDetectedCallback = callback; }

    int commandsPublished{};
    BrakeCommand lastCommand;
    SpeedUpdateCallback speedUpdateCallback;
    CarDetectedCallback carDetectedCallback;
};

struct AutoBrake {
    AutoBrake(IServiceBus& bus) : speed_mps{}, collision_threshold_s{5} {
        bus.subscribe([this](const SpeedUpdate& update) { speed_mps = update.velocity_mps; });

        bus.subscribe([this, &bus](const CarDetected& update) {
            const auto relative_velocity_mps = speed_mps - update.velocity_mps;
            const auto time_to_collision_s = update.distance_m / relative_velocity_mps;
            if (time_to_collision_s > 0 && time_to_collision_s <= collision_threshold_s) {
                bus.publish(BrakeCommand{time_to_collision_s});
            }
        });
    }

    void setCollisionThresholdS(double x) {
        if (x < 1) throw std::runtime_error{"Collision less than 1"};
        collision_threshold_s = x;
    }

    double getCollisionThresholdS() const { return collision_threshold_s; }

    double getSpeedMPS() const { return speed_mps; }

   private:
    double collision_threshold_s;
    double speed_mps;
};
