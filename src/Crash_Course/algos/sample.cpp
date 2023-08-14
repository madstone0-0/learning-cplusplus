/* Generates random, stable sequences
 */
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>

using namespace std;

const string pop = "ABCD";
const size_t n_samples{1'000'000};

mt19937_64 urbg;

void sample_length(size_t n) {
    cout << "--Length " << n << "--\n";
    map<string, size_t> counts;

    for (size_t i{}; i < n_samples; i++) {
        string res;
        sample(pop.begin(), pop.end(), back_inserter(res), n, urbg);
        counts[res]++;
    }

    for (const auto [sample, n] : counts) {
        const auto percentage = 100 * n / static_cast<double>(n_samples);
        cout << percentage << " '" << sample << "'"
             << "\n";
    }
}

int main() {
    cout << fixed << setprecision(1);
    sample_length(0);
    sample_length(1);
    sample_length(2);
    sample_length(3);
    sample_length(4);

    return 0;
}
