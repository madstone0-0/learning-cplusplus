#ifndef ISERVICE
#define ISERVICE
#include <functional>

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

struct SpeedLimitDetected {
    unsigned short speed_mps;
};

using SpeedUpdateCallback = std::function<void(const SpeedUpdate&)>;
using CarDetectedCallback = std::function<void(const CarDetected&)>;
using SpeedLimitDetectedCallback = std::function<void(const SpeedLimitDetected&)>;

struct IServiceBus {
    virtual ~IServiceBus() = default;
    virtual void publish(const BrakeCommand&) = 0;
    virtual void subscribe(SpeedUpdateCallback) = 0;
    virtual void subscribe(CarDetectedCallback) = 0;
    virtual void subscribe(SpeedLimitDetectedCallback) = 0;
};

#endif
