#include <iostream>
#include <sstream>
#include <string>

#include "../unit_testing/car.cpp"
#include "../unit_testing/mock_og.cpp"

using namespace std;

ostream& operator<<(ostream& s, const AutoBrake& brake) {
    s << "Current Collision Threshold: " << brake.getCollisionThresholdS() << "\n";
    s << "Current speed: " << brake.getSpeedMPS() << endl;
    return s;
}

int main() {
    MockServiceBus bus;
    AutoBrake brake{bus};

    bus.speedUpdateCallback(SpeedUpdate{40L});
    cout << brake << "\n";

    bus.speedUpdateCallback(SpeedUpdate{100L});
    brake.setCollisionThresholdS(2);
    cout << brake << "\n";

    bus.carDetectedCallback(CarDetected{10, 0L});
    cout << brake << endl;

    return 0;
}
