#define CATCH_CONFIG_MAIN
#include "utils.hpp"

#include <array>
#include <catch2/catch.hpp>
#include <exception>

using std::array, Utils::sliceArray;

TEST_CASE("utils::slice works on std::array") {
    std::array<int, 10> arr{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    SECTION("slice [2, 5)") {
        array<int, 3> res{3, 4, 5};
        array<int, 3> slice = sliceArray<decltype(arr), decltype(slice)>(arr, 2, 5);
        REQUIRE(res == slice);
    }

    SECTION("start and end parameters cannot be the same") {
        array<int, 3> slice;
        REQUIRE_THROWS((slice = sliceArray<decltype(arr), decltype(slice)>(arr, 3, 2)));
    }

    SECTION("out of bounds slice throws") {
        array<int, 3> slice;
        REQUIRE_THROWS((slice = sliceArray<decltype(arr), decltype(slice)>(arr, 14, 16)));
    }
}

// TEST_CASE("utils::slice works on c style arrays") {
//     int cArr[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//     SECTION("slice [1, 7)") {
//         const int res[]{2, 3, 4, 5, 6, 7};
//         int* slice = sliceArray<decltype(cArr), int[6], 10>(&cArr, 1, 7);
//         REQUIRE(slice == res);
//     }
// }
