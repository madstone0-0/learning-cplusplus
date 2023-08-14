#include <stdexcept>

#include "iservice.cpp"

#ifndef CAR
#define CAR

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

#endif
