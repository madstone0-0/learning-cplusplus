#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T>
struct Matrix {
    Matrix(size_t rows, size_t columns, std::initializer_list<T> val)
        : dim{rows, columns}, data(dim.first, std::vector<T>{}) {
        if (rows * columns != val.size()) throw std::logic_error{"Not enough items to fill matrix"};
        auto itr = val.begin();
        for (size_t row{}; row < dim.first; row++) {
            if (dim.second + itr != val.end()) {
                data[row].assign(itr, itr + dim.second);
            }
            itr += dim.second;
        }
    }

    T& at(size_t row, size_t col) {
        if (row >= dim.first || col >= dim.second) throw std::out_of_range("Index invalid");
        return data[row][col];
    }

    const std::pair<size_t, size_t> dim{};

   private:
    std::vector<std::vector<T>> data{};
};

TEST_CASE("Matrix works") {
    Matrix<int> matrix{3, 5, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0, 0}};

    REQUIRE(matrix.dim.first == 3);
    REQUIRE(matrix.dim.second == 5);
    matrix.at(0, 4) = 7;
    REQUIRE(matrix.at(0, 4) == 7);
    REQUIRE(matrix.at(1, 3) == 9);
    REQUIRE_THROWS(Matrix{1, 2, {2}});
}
