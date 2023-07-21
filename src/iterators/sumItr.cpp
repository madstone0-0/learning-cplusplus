#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <iterator>
#include <typeinfo>
#include <vector>

template <typename T>
concept Iterator = requires(T a) {
    *(a);
    a++;
};

template <Iterator Iter>
int sumItr(Iter begin, Iter end) {
    int sum{};
    while (begin != end) sum += *(begin++);
    return sum;
}

TEST_CASE("sumItr works") {
    std::vector<int> nums(10, 10);
    auto res1 = sumItr(nums.begin(), nums.end());

    REQUIRE(res1 == 100);

    auto res2 = sumItr(nums.crbegin(), nums.crend());

    REQUIRE(res2 == 100);
}
