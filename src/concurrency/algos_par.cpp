#include <algorithm>
#include <atomic>
#include <execution>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<long> numbers(1'000'000), squares(1'000'000);
    std::iota(numbers.begin(), numbers.end(), 0);
    std::atomic_size_t n_transformed{};

    std::transform(std::execution::par, numbers.begin(), numbers.end(), squares.begin(),
                   [&n_transformed](const auto x) {
                       n_transformed++;
                       return x * x;
                   });
    std::cout << "n_transformed: " << n_transformed << std::endl;
}
