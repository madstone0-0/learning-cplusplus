#include <cstdio>

// Check for error conditions upon calling
struct HumptyDumpty {
    HumptyDumpty(){};

    bool isTogetherAgain();
};
//
// bool sendKingsHorsesAndMen() {
//     HumptyDumpty hd{};
//     if (hd.isTogetherAgain()) return false;
//     return true;
// }

// Return the value and an error code
struct Result {
    HumptyDumpty hd;
    bool success;
};

Result makeHumpty() {
    HumptyDumpty hd{};
    bool isValid;
    return {hd, isValid};
}

bool sendKingsHorsesAndMen() {
    auto [hd, success] = makeHumpty();
    if (!success) return false;
    return true;
}

int main() { return 0; }
