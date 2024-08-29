#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <iterator>
#include <numeric>
#include <optional>
#include <ostream>
#include <ranges>
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

        [[nodiscard]] size_t size() const { return n; }

        Vector<number, n> operator*(const number& scalar) const {
            Vector<number, n> res{};
            for (size_t i{}; i < n; i++) res.at(i) = scalar * this->at(i);
            return res;
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

        friend std::ostream& operator<<(std::ostream& os, const Vector<number, n>& row) {
            if (row.size() == 1)
                os << "[ " << row.at(0) << " ]\n";
            else
                for (size_t i{}; i < row.size(); i++)
                    if (i == 0) {
                        os << "⎡ " << row.at(i) << " ⎤\n";
                    } else if (i == row.size() - 1) {
                        os << "⎣ " << row.at(i) << " ⎦\n";
                    } else {
                        os << "| " << row.at(i) << " |\n";
                    }
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
                std::string val{};
                if (rowVal.has_value())
                    val = std::to_string(rowVal.value());
                else
                    val = "None";
                return val;
            };

            if (size == 1)
                os << "[ " << (row.at(0).has_value() ? std::string{row.at(0).value()} : "None") << " ]\n";
            else
                for (size_t i{}; i < row.size(); i++) {
                    if (i == 0) {
                        os << "⎡ " << hasVal(row.at(i)) << " ⎤\n";
                    } else if (i == row.size() - 1) {
                        os << "⎣ " << hasVal(row.at(i)) << " ⎦\n";

                    } else {
                        os << "| " << hasVal(row.at(i)) << " |\n";
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
            return multMatByDef(extractMatrixFromTranspose(other));
        }

        Matrix& operator=(Matrix&& other) noexcept {
            matrix = std::move(other.matrix);
            return *this;
        }

        ~Matrix() = default;

        [[nodiscard]] size_t numRows() const { return m; }

        [[nodiscard]] size_t numCols() const { return m; }

        friend std::ostream& operator<<(std::ostream& os, const Matrix<number, m, n>& system) {
            const auto& nRows = system.numRows();
            if (system.numRows() == 1) {
                os << "[ ";
                int i{};
                for (const auto& elem : system.at(0)) os << " " << elem << " ";
                os << "]\n";
            } else {
                int i{};
                for (const auto& row : system.matrix) {
                    if (i == 0) {
                        os << "⎡";
                        for (const auto& elem : row) os << " " << elem << " ";
                        os << "⎤\n";
                    } else if (i == nRows - 1) {
                        os << "⎣";
                        for (const auto& elem : row) os << " " << elem << " ";
                        os << "⎦\n";
                    } else {
                        os << "|";
                        for (const auto& elem : row) os << " " << elem << " ";
                        os << "|\n";
                    }
                    i++;
                }
            }
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

        template <Number num, size_t mN, size_t nN>
        static Matrix<num, mN, nN> fromColVectorSet(const vector<Vector<num, mN>>& vecSet) {
            Matrix<num, mN, nN> res;
            for (size_t i{}; i < nN; i++) {
                const auto& vec{vecSet.at(i)};
                for (size_t j{}; j < mN; j++) {
                    res.at(j).at(i) = vec.at(j);
                }
            }
            return res;
        }

        template <Number num, size_t mN, size_t nN>
        static Matrix<num, mN, nN> fromRowVectorSet(const vector<Vector<num, nN>>& vecSet) {
            Matrix<num, mN, nN> res;
            for (size_t i{}; i < mN; i++) {
                res.at(i) = vecSet.at(i);
            }
            return res;
        }

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

       private:
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
            return Matrix::fromColVectorSet<number, m, nOther>(res);
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
