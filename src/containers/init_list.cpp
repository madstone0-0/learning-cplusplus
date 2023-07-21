#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <cmath>
#include <initializer_list>
#include <stdexcept>
#include <vector>

size_t squareRoot(size_t x) {
    const auto res = static_cast<size_t>(sqrt(x));
    if (res * res != x) throw std::logic_error{"Not a perfect square"};
    return res;
}

template <typename T>
struct SquareMatrix {
    SquareMatrix(std::initializer_list<T> val) : dim{squareRoot(val.size())}, data(dim, std::vector<T>{}) {
        auto itr = val.begin();
        for (size_t row{}; row < dim; row++) {
            data[row].assign(itr, itr + dim);
            itr += dim;
        }
    }

    T& at(size_t row, size_t col) {
        if (row >= dim || col >= dim) throw std::out_of_range{"Index invalid"};
        return data[row][col];
    }
    const size_t dim;

   private:
    std::vector<std::vector<T>> data;
};

/* std::Initializer_List */

TEST_CASE("SquareMatrix and std::initializer_list") {
    SquareMatrix<int> mat{1, 2, 3, 4, 5, 0, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    REQUIRE(mat.dim == 4);
    mat.at(1, 1) = 6;
    REQUIRE(mat.at(1, 1) == 6);
    REQUIRE(mat.at(2, 0) == 9);
}

/* std::Initializer_List */
