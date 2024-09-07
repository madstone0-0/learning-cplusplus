#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <optional>
#include <ostream>
#include <ranges>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

template <typename T>
concept Number = requires {
    std::is_integral_v<T> || std::is_floating_point_v<T>;
    std::is_convertible_v<T, std::string>;
};

using std::vector, std::array, std::optional;
namespace rg = std::ranges;

namespace mlinalg::structures {
    template <Number number, size_t m, size_t n>
    class Matrix;

    template <Number number, size_t n>
    class Vector;

    // template <Number number, size_t n>
    // using ColumnVector = Matrix<number, n, 1>;

    template <Number number, size_t m, size_t n>
    using TransposeVariant = std::variant<Vector<number, m>, Matrix<number, n, m>>;

    namespace helpers {
        template <Number num, size_t m, size_t n>
        Matrix<num, m, n> fromColVectorSet(const vector<Vector<num, m>>& vecSet) {
            Matrix<num, m, n> res;
            for (size_t i{}; i < n; i++) {
                const auto& vec{vecSet.at(i)};
                for (size_t j{}; j < m; j++) {
                    res.at(j).at(i) = vec.at(j);
                }
            }
            return res;
        }

        template <Number num, size_t m, size_t n>
        Matrix<num, m, n> fromRowVectorSet(const vector<Vector<num, n>>& vecSet) {
            Matrix<num, m, n> res;
            for (size_t i{}; i < m; i++) {
                res.at(i) = vecSet.at(i);
            }
            return res;
        }

        template <Number num, size_t m, size_t n>
        Matrix<num, n, m> extractMatrixFromTranspose(const TransposeVariant<num, m, n> T) {
            return std::get<Matrix<num, n, m>>(T);
        }

        template <Number num, size_t m, size_t n>
        Matrix<num, n, m> extractVectorFromTranspose(const TransposeVariant<num, m, n> T) {
            return std::get<Vector<num, m>>(T);
        }

    }  // namespace helpers

    /**
     * @brief Vector class for represeting both row and column vectors in n-dimensional space
     */
    template <Number number, size_t n>
    class Vector {
       public:
        Vector() = default;
        Vector(std::initializer_list<number> list) {
            for (size_t i{}; i < n; i++) row.at(i) = *(list.begin() + i);
        }

        Vector(const Vector& other) : row{other.row} {}

        Vector(Vector&& other) noexcept : row{std::move(other.row)} {}

        Vector& operator=(const Vector& other) {
            if (this == &other) return *this;
            row = other.row;
            return *this;
        }

        Vector& operator=(const Matrix<number, 1, n>& other) {
            for (size_t i{}; i < n; i++) row.at(i) = other.at(0).at(i);
            return *this;
        }

        Vector& operator=(Vector&& other) noexcept {
            row = std::move(other.row);
            return *this;
        }

        ~Vector() = default;

        number& at(size_t i) { return row.at(i); }
        number at(size_t i) const { return row.at(i); }

        double dot(Vector<number, n> other) const { return (this->T() * other).at(0); }

        [[nodiscard]] double length() const { return std::sqrt(this->dot(*this)); }

        [[nodiscard]] double dist(Vector<number, n> other) const {
            auto diff = *this - other;
            return std::sqrt(diff.dot(diff));
        }

        Vector<number, n> operator-(Vector<number, n> other) const {
            Vector<number, n> res{};
            for (size_t i{}; i < n; i++) res.at(i) = row.at(i) - other.at(i);
            return res;
        }
        Vector<number, n> operator+(Vector<number, n> other) const {
            Vector<number, n> res{};
            for (size_t i{}; i < n; i++) res.at(i) = row.at(i) + other.at(i);
            return res;
        }

        Vector<number, n>& operator+=(Vector<number, n> other) {
            for (size_t i{}; i < n; i++) this->at(i) += other.at(i);
            return *this;
        }

        [[nodiscard]] size_t size() const { return n; }

        Vector<number, n> operator*(const number& scalar) const {
            Vector<number, n> res{};
            for (size_t i{}; i < n; i++) res.at(i) = scalar * this->at(i);
            return res;
        }

        Vector<number, n> operator/(const number& scalar) const {
            Vector<number, n> res{};
            for (size_t i{}; i < n; i++) res.at(i) = this->at(i) / scalar;
            return res;
        }

        Vector<number, n>& operator*=(const number& scalar) {
            for (size_t i{}; i < n; i++) this->at(i) *= scalar;
            return *this;
        }

        template <size_t m>
        Vector<number, n> operator*(Matrix<number, m, n> mat) {
            Vector<number, n> res{};
            auto asCols{mat.colToVectorSet()};
            for (size_t i{}; i < n; i++) {
                auto multRes = *this * asCols.at(i);
                res.at(i) = std::accumulate(multRes.begin(), multRes.end(), 0);
            }
            return res;
        }

        /**
         * @brief Transpose a row vector to a column vector
         *
         * @return ColumnVector<number, n>
         */
        [[nodiscard]] Matrix<number, 1, n> T() const {
            Matrix<number, 1, n> res{};
            for (size_t i{}; i < n; i++) res.at(0).at(i) = this->at(i);
            return res;
        }

        auto operator*(Vector<number, n> vec) const { return this->T() * vec; }

        constexpr auto begin() const { return row.begin(); }
        constexpr auto end() const { return row.end(); }
        constexpr auto cbegin() const { return row.cbegin(); }
        constexpr auto cend() const { return row.cend(); }

        constexpr auto& back() { return row.back(); }
        constexpr auto& back() const { return row.back(); }

        constexpr auto rbegin() { return row.rbegin(); }
        constexpr auto rend() { return row.rend(); }

        explicit operator std::string() const {
            std::stringstream ss{};

            size_t maxWidth = 0;
            for (const auto& elem : row) {
                std::stringstream temp_ss;
                temp_ss << elem;
                maxWidth = std::max(maxWidth, temp_ss.str().length());
            }

            if (row.size() == 1)
                ss << "[ " << std::setw(maxWidth) << row.at(0) << " ]\n";
            else
                for (size_t i{}; i < row.size(); i++)
                    if (i == 0) {
                        ss << "⎡ " << std::setw(maxWidth) << row.at(i) << " ⎤\n";
                    } else if (i == row.size() - 1) {
                        ss << "⎣ " << std::setw(maxWidth) << row.at(i) << " ⎦\n";
                    } else {
                        ss << "| " << std::setw(maxWidth) << row.at(i) << " |\n";
                    }
            return ss.str();
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector<number, n>& row) {
            os << std::string(row);
            return os;
        }

        friend std::ostream& operator<<(std::ostream& os, const optional<Vector<number, n>>& rowPot) {
            if (!rowPot.has_value()) {
                os << "Empty Vector";
                return os;
            }
            const auto& row = rowPot.value();
            const auto& size = row.size();

            auto hasVal = [](auto rowVal) {
                std::stringstream val;
                if (rowVal.has_value())
                    val << rowVal.value();
                else
                    val << "None";
                return val.str();
            };

            size_t maxWidth = 0;
            for (const auto& elem : row) {
                std::stringstream temp_ss;
                temp_ss << hasVal(elem);
                maxWidth = std::max(maxWidth, temp_ss.str().length());
            }

            if (size == 1) {
                os << "[ ";
                if (row.at(0).has_value())
                    os << row.at(0).value();
                else
                    os << "None";
                os << " ]\n";
            } else
                for (size_t i{}; i < row.size(); i++) {
                    if (i == 0) {
                        os << "⎡ " << std::setw(maxWidth) << hasVal(row.at(i)) << " ⎤\n";
                    } else if (i == row.size() - 1) {
                        os << "⎣ " << std::setw(maxWidth) << hasVal(row.at(i)) << " ⎦\n";

                    } else {
                        os << "| " << std::setw(maxWidth) << hasVal(row.at(i)) << " |\n";
                    }
                }
            return os;
        }

       private:
        friend void swap(Vector& first, Vector& second) noexcept {
            using std::swap;
            swap(first.row, second.row);
        }

        array<number, n> row{};
    };

    template <Number number, size_t n>
    Vector<number, n> operator*(const number& scalar, Vector<number, n> vec) {
        return vec * scalar;
    }

    template <Number number>
    using Vector2 = mlinalg::structures::Vector<number, 2>;

    template <Number number>
    using Vector3 = mlinalg::structures::Vector<number, 3>;

    template <Number number, size_t n>
    using Row = Vector<number, n>;

    template <Number number, size_t m, size_t n>
    class Matrix {
       public:
        Row<number, n>& at(size_t i) { return matrix.at(i); }
        Row<number, n> at(size_t i) const { return matrix.at(i); }

        Matrix() = default;
        constexpr Matrix(std::initializer_list<std::initializer_list<number>> rows) {
            for (size_t i{}; i < m; i++) matrix.at(i) = Row<number, n>{*(rows.begin() + i)};
        }

        Matrix(const Matrix& other) : matrix{other.matrix} {}
        Matrix(Matrix&& other) noexcept : matrix{std::move(other.matrix)} {}

        Matrix& operator=(const Matrix& other) {
            if (this == &other) return *this;
            matrix = other.matrix;
            return *this;
        }

        vector<Vector<number, m>> colToVectorSet() const {
            vector<Vector<number, m>> res{};
            for (size_t i{}; i < n; i++) {
                Vector<number, m> vec;
                for (size_t j{}; j < m; j++) {
                    vec.at(j) = this->matrix.at(j).at(i);
                }
                res.push_back(vec);
            }
            return res;
        }

        vector<Vector<number, n>> rowToVectorSet() const {
            vector<Vector<number, n>> res;
            res.reserve(m);
            for (const auto& row : matrix) res.push_back(row);
            return res;
        }

        Matrix operator*(const number& scalar) const {
            Matrix<number, m, n> res{};
            auto asRowVectorSet{rowToVectorSet()};
            for (size_t i{}; i < m; i++) {
                res.at(i) = asRowVectorSet.at(i) * scalar;
            }
            return res;
        }

        Matrix operator+(Matrix<number, m, n> other) const {
            Matrix<number, m, n> res{};
            for (size_t i{}; i < m; i++) res.at(i) = this->at(i) + other.at(i);
            return res;
        }

        Matrix operator-(Matrix<number, m, n> other) const {
            Matrix<number, m, n> res{};
            for (size_t i{}; i < m; i++) res.at(i) = this->at(i) + other.at(i);
            return res;
        }

        Vector<number, m> operator*(Vector<number, n> vec) const { return multMatByVec(vec); }

        template <size_t nOther>
        Matrix<number, m, nOther> operator*(const Matrix<number, n, nOther> other) const {
            return multMatByDef(other);
        }

        template <size_t nOther>
        Matrix<number, m, nOther> operator*(const TransposeVariant<number, n, nOther> other) const {
            return multMatByDef(helpers::extractMatrixFromTranspose(other));
        }

        Matrix& operator=(Matrix&& other) noexcept {
            matrix = std::move(other.matrix);
            return *this;
        }

        ~Matrix() = default;

        [[nodiscard]] size_t numRows() const { return m; }

        [[nodiscard]] size_t numCols() const { return m; }

        explicit operator std::string() const {
            std::stringstream ss{};
            const auto& nRows = this->numRows();

            size_t maxWidth = 0;
            for (const auto& row : matrix) {
                for (const auto& elem : row) {
                    std::stringstream temp_ss;
                    temp_ss << elem;
                    maxWidth = std::max(maxWidth, temp_ss.str().length());
                }
            }

            if (this->numRows() == 1) {
                ss << "[ ";
                int i{};
                for (const auto& elem : this->at(0)) ss << " " << std::setw(maxWidth) << elem << " ";
                ss << "]\n";
            } else {
                int i{};
                for (const auto& row : this->matrix) {
                    if (i == 0) {
                        ss << "⎡";
                        for (const auto& elem : row) ss << " " << std::setw(maxWidth) << elem << " ";
                        ss << "⎤\n";
                    } else if (i == nRows - 1) {
                        ss << "⎣";
                        for (const auto& elem : row) ss << " " << std::setw(maxWidth) << elem << " ";
                        ss << "⎦\n";
                    } else {
                        ss << "|";
                        for (const auto& elem : row) ss << " " << std::setw(maxWidth) << elem << " ";
                        ss << "|\n";
                    }
                    i++;
                }
            }
            return ss.str();
        }

        friend std::ostream& operator<<(std::ostream& os, const Matrix<number, m, n>& system) {
            os << std::string(system);
            return os;
        }

        TransposeVariant<number, m, n> T() const {
            TransposeVariant<number, m, n> res;

            auto mutateMatrix = [this](auto& variant) {
                if constexpr (std::is_same_v<std::decay_t<decltype(variant)>, Matrix<number, n, m>>) {
                    for (size_t i{}; i < m; i++)
                        for (size_t j{}; j < n; j++) variant.at(j).at(i) = this->matrix.at(i).at(j);
                }
            };

            auto mutateVector = [this](auto& variant) {
                if constexpr (std::is_same_v<std::decay_t<decltype(variant)>, Vector<number, m>>) {
                    for (size_t i{}; i < m; i++) variant.at(i) = this->matrix.at(i).at(0);
                }
            };

            if (n != 1) {
                res = Matrix<number, n, m>{};
                std::visit(mutateMatrix, res);
                return res;
            }

            res = Vector<number, m>{};
            std::visit(mutateVector, res);
            return res;
        }

        template <Number num, size_t mN, size_t nN>
        static Matrix<num, nN, mN> extractMatrixFromTranspose(const TransposeVariant<num, mN, nN> T) {
            return std::get<Matrix<num, nN, mN>>(T);
        }

        /*template <Number num, size_t mN, size_t nN>*/
        friend std::ostream& operator<<(std::ostream& os, const TransposeVariant<number, n, m>& system) {
            if (std::holds_alternative<Vector<number, n>>(system)) {
                os << std::get<Vector<number, n>>(system);
            } else if (std::holds_alternative<Matrix<number, m, n>>(system)) {
                os << std::get<Matrix<number, m, n>>(system);
            }
            return os;
        }

        constexpr auto begin() const { return matrix.begin(); }
        constexpr auto end() const { return matrix.end(); }
        constexpr auto cbegin() const { return matrix.cbegin(); }
        constexpr auto cend() const { return matrix.cend(); }

        constexpr auto back() { return matrix.back(); }

        constexpr auto rbegin() { return matrix.rbegin(); }
        constexpr auto rend() { return matrix.rend(); }

        template <size_t nN>
        Matrix<number, m, nN + n> augment(const Matrix<number, m, nN>& other) const {
            Matrix<number, m, nN + n> res{};
            for (int i{}; i < m; i++) {
                auto& row{res.at(i)};
                const auto& thisRow{this->at(i)};
                const auto& otherRow{other.at(i)};
                for (int j{}; j < (nN + n); j++) {
                    if (j < n)
                        row.at(j) = thisRow.at(j);
                    else
                        row.at(j) = otherRow.at((j - static_cast<int>(nN)));
                }
            }
            return res;
        }

        Matrix<number, m, n + 1> augment(const Vector<number, m>& other) const {
            Matrix<number, m, n + 1> res{};
            for (size_t i{}; i < m; i++) {
                auto& row{res.at(i)};
                const auto& thisRow{this->at(i)};
                size_t j{};
                for (; j < n; j++) {
                    row.at(j) = thisRow.at(j);
                }
                row.at(j) = other.at(i);
            }
            return res;
        }

        number det() const {
            if (m != n) throw std::runtime_error("Finding determinant of rectangular matrices not implemented");
            if constexpr (m == 2 && n == 2)
                return det2x2();
            else
                return cofactor();
        }

        Matrix<number, m - 1, n - 1> subset(std::optional<size_t> i, std::optional<size_t> j) const {
            static_assert(m == n, "Not a square matrix");
            static_assert(m > 1 && n > 1, "Cannot subset");

            Matrix<number, m - 1, n - 1> res{};
            size_t resRow = 0;
            for (size_t k = 0; k < m; ++k) {
                if (i.has_value() && i.value() == k) continue;  // Skip the row to be removed

                size_t resCol = 0;
                for (size_t z = 0; z < n; ++z) {
                    if (j.has_value() && j.value() == z) continue;  // Skip the column to be removed

                    res.at(resRow).at(resCol) = this->at(k).at(z);
                    ++resCol;
                }
                ++resRow;
            }
            return res;
        }

       private:
        number det2x2() const {
            return (this->at(0).at(0) * this->at(1).at(1)) - (this->at(0).at(1) * this->at(1).at(0));
        }

        enum By { ROW = 0, COL };

        std::pair<By, size_t> pickCofactorRowOrCol() const {
            size_t maxRowZeros{};
            size_t rowPos{};

            size_t maxColZeros{};
            size_t colPos{};

            int pos{};
            for (const auto& row : *this) {
                auto count = rg::count_if(row, [](auto x) { return x == 0; });
                if (count > maxRowZeros) {
                    maxRowZeros = count;
                    rowPos = pos;
                }
                pos++;
            }

            pos = 0;
            for (const auto& col : colToVectorSet()) {
                auto count = rg::count_if(col, [](auto x) { return x == 0; });
                if (count > maxColZeros) {
                    maxColZeros = count;
                    colPos = pos;
                }
                pos++;
            }

            if (maxColZeros >= maxRowZeros)
                return {COL, colPos};
            else
                return {ROW, rowPos};
        }

        constexpr number cofactorCommon(size_t i, size_t j) const {
            number res{};
            auto a = this->at(i).at(j);
            if (a == 0) return 0;
            auto A_ij = this->subset(i, j);
            auto C = ((int)std::pow(-1, ((i + 1) + (j + 1)))) * A_ij.det();
            res += a * C;
            return res;
        }

        constexpr number cofactorRow(size_t row) const {
            number res{};
            size_t i{row};
            for (size_t j{0}; j < n; j++) {
                res += cofactorCommon(i, j);
            }
            return res;
        }

        constexpr number cofactorCol(size_t col) const {
            number res{};
            size_t j{col};
            for (size_t i{0}; i < n; i++) {
                res += cofactorCommon(i, j);
            }
            return res;
        }

        constexpr number cofactor() const {
            auto [by, val] = pickCofactorRowOrCol();
            if (by == ROW)
                return cofactorRow(val);
            else
                return cofactorCol(val);
        }

        Vector<number, m> multMatByVec(Vector<number, n> vec) const {
            Vector<number, m> res{};
            auto asCols{colToVectorSet()};
            int i{};
            for (auto& col : asCols) {
                const auto& mult = vec.at(i);
                col = mult * col;
                i++;
            }

            for (size_t i{}; i < m; i++) {
                number sumRes{};
                for (const auto& col : asCols) sumRes += col.at(i);
                res.at(i) = sumRes;
            }

            return res;
        }

        template <size_t nOther>
        Matrix<number, m, nOther> multMatByDef(const Matrix<number, n, nOther>& other) const {
            auto otherColVecSet{other.colToVectorSet()};
            vector<Vector<number, m>> res{};
            for (size_t i{}; i < nOther; i++) {
                const auto& col{otherColVecSet.at(i)};
                auto multRes = *this * col;
                res.push_back(multRes);
            }
            return helpers::fromColVectorSet<number, m, nOther>(res);
        }

        friend void swap(Matrix& first, Matrix& second) noexcept {
            using std::swap;
            swap(first.matrix, second.matrix);
        }

        array<Row<number, n>, m> matrix{};
    };

    // Deduction Guide
    template <typename T, typename... Rows>
    Matrix(T, Rows...)
        -> Matrix<T, sizeof...(Rows), std::tuple_size_v<std::decay_t<decltype(std::get<0>(std::tuple<Rows...>()))>>>;

    template <typename T, size_t Rows, size_t Cols>
    Matrix(const std::array<std::array<T, Cols>, Rows>&) -> Matrix<T, Rows, Cols>;

    // Alias template for LinearSystem

    template <Number number, size_t n>
    Vector(std::array<number, n>) -> Vector<number, n>;

    template <Number number, size_t m, size_t n, size_t nOther>
    Matrix<number, m, nOther> operator*(Matrix<number, m, n> lhs, Matrix<number, n, nOther> rhs) {
        return lhs * rhs;
    }

    template <Number number, size_t m, size_t n, size_t nOther>
    TransposeVariant<number, m, nOther> operator*(TransposeVariant<number, m, n> lhs, Matrix<number, n, nOther> rhs) {
        if (std::holds_alternative<Vector<number, m>>(lhs)) {
            auto vec = std::get<Vector<number, m>>(lhs);
            return vec * rhs;
        } else {
            auto mat = std::get<Matrix<number, m, n>>(lhs);
            return mat * rhs;
        }
    }

    template <Number number, size_t m, size_t n>
    Matrix<number, m, n> operator*(const number& scalar, Matrix<number, m, n> rhs) {
        return rhs * scalar;
    }

    template <Number number, size_t m, size_t n>
    Matrix<number, m, n> operator*(Vector<number, n> vec, Matrix<number, m, n> rhs) {
        Matrix<number, m, n> res{};
        for (size_t i{}; i < n; i++) {
            const auto& mult = vec.at(i);
            for (size_t j{}; j < m; j++) res.at(j).at(i) = mult * rhs.matrix.at(j).at(i);
        }
        return res;
    }

}  // namespace mlinalg::structures
