#include <iostream>
#include <random>
#include <vector>

long rng(const auto& max, const auto& min) {
    std::random_device dev;
    std::mt19937 mtEngine{dev()};
    std::uniform_int_distribution<long> intL{min, max};
    return intL(mtEngine);
}

int main(int argc, char* argv[]) {
    using namespace std;
    long MAX{}, MIN{};
    long SAMPLES{};

    cout << "Emter sample size: ";
    cin >> SAMPLES;

    cout << "Enter MAX: ";
    cin >> MAX;

    cout << "Enter MIN: ";
    cin >> MIN;

    if (MIN >= MAX) {
        cerr << "MAX cannot be greater than or equal to MIN" << endl;
        return 1;
    }

    vector<long> samples{};
    for (int i{}; i < SAMPLES; i++) {
        samples.push_back(rng(MAX, MIN));
    }

    auto countItems = [&samples](long item) {
        long count{};
        for (const auto& i : samples)
            if (i == item) count++;
        return count;
    };

    for (auto i{MIN}; i <= MAX; i++) {
        cout << i << ": ";
        auto count{countItems(i)};
        while (count--) cout << "*";
        cout << "\n";
    }

    return 0;
}
