#pragma once

#include <array>
#include <optional>
#include <ostream>
#include <set>
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

namespace mlinalg::structures {
    template <Number number, size_t m, size_t n>
    class Matrix;

    template <Number number, size_t n>
    using ColumnVector = Matrix<number, n, 1>;

    template <Number number, size_t n>
    class Vector {
       public:
        Vector() = default;
        Vector(std::initializer_list<number> list) {
            /*if constexpr (list.size() != n) throw std::invalid_argument("Invalid vector size");*/
            for (size_t i{}; i < n; i++) row.at(i) = *(list.begin() + i);
        }

        Vector(const Vector& other) : row{other.row} {}

        Vector(Vector&& other) noexcept : row{std::move(other.row)} {}

        Vector& operator=(const Vector& other) {
            if (this == &other) return *this;
            row = other.row;
            return *this;
        }

        Vector& operator=(Vector&& other) noexcept {
            row = std::move(other.row);
            return *this;
        }

        ~Vector() = default;

        number& at(size_t i) { return row.at(i); }
        number at(size_t i) const { return row.at(i); }

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

        template <Number Scalar>
        Vector<number, n> operator*(Scalar scalar) const {
            Vector<number, n> res{};
            for (size_t i{}; i < n; i++) res.at(i) = scalar * this->at(i);
            return res;
        }

        [[nodiscard]] ColumnVector<number, n> T() const {
            ColumnVector<number, n> res{};
            for (size_t i{}; i < n; i++) res.at(i).at(0) = this->at(i);
            return res;
        }

        Vector operator*(ColumnVector<number, n> vec) const {
            Vector<number, n> res{};
            for (size_t i{}; i < n; i++) res.at(i) = this->at(i) * vec.at(i).at(0);
            return res;
        }

        template <Number Scalar>
        friend Vector<number, n> operator*(Scalar scalar, Vector<number, n> vec) {
            return vec * scalar;
        }

        constexpr auto begin() const { return row.begin(); }
        constexpr auto end() const { return row.end(); }
        constexpr auto cbegin() const { return row.cbegin(); }
        constexpr auto cend() const { return row.cend(); }

        constexpr auto& back() { return row.back(); }
        constexpr auto& back() const { return row.back(); }

        constexpr auto rbegin() { return row.rbegin(); }
        constexpr auto rend() { return row.rend(); }

        friend std::ostream& operator<<(std::ostream& os, const optional<Vector<number, n>>& rowPot) {
            os << "[";
            if (!rowPot.has_value()) {
                os << "Empty Vector";
                return os;
            }
            const auto& row = rowPot.value();

            for (size_t i{}; i < row.size(); i++)
                if (i != row.size() - 1) {
                    if (row.at(i).has_value())
                        os << row.at(i).value() << ", ";
                    else
                        os << "None, ";

                } else {
                    if (row.at(i).has_value())
                        os << row.at(i).value();
                    else
                        os << "None";
                }
            os << " ]\n\n";
            return os;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector<number, n>& row) {
            os << "[";
            for (size_t i{}; i < row.size(); i++)
                if (i != row.size() - 1) {
                    os << row.at(i) << ", ";

                } else {
                    os << row.at(i);
                }
            os << " ]\n";
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
    using Row = Vector<number, n>;

    template <Number number, size_t m, size_t n>
    using TransposeVariant = std::variant<Vector<number, m>, Matrix<number, n, m>>;

    template <Number number, size_t m, size_t n>
    class Matrix {
       public:
        Row<number, n>& at(size_t i) { return matrix.at(i); }
        Row<number, n> at(size_t i) const { return matrix.at(i); }

        Matrix() = default;
        constexpr Matrix(std::initializer_list<std::initializer_list<number>> rows) {
            /*if constexpr (rows.size() != m) throw std::invalid_argument("Invalid matrix size");*/
            for (size_t i{}; i < m; i++) matrix.at(i) = Row<number, n>{*(rows.begin() + i)};
        }
        /*Matrix(array<Row<number, n>, m> rows) : matrix{rows} {*/
        /*    if constexpr (rows.size() != m || rows.at(0).size != n) throw std::invalid_argument("Invalid matrix
         * size");*/
        /*}*/

        /*template <typename... Rows>*/
        /*constexpr Matrix(Rows&&... rows) : matrix{std::forward<Rows>(rows)...} {*/
        /*    static_assert((std::conjunction_v<std::bool_constant<std::tuple_size_v<std::decay_t<Rows>> == n>...>),*/
        /*                  "Invalid matrix size");*/
        /*}*/

        /*template <typename... Rows, typename = std::enable_if_t<(sizeof...(Rows) == m)>>*/
        /*constexpr explicit Matrix(Rows&&... rows) : matrix({std::forward<Rows>(rows)...}) {*/
        /*    static_assert((std::conjunction_v<std::bool_constant<std::tuple_size_v<std::decay_t<Rows>> == n>...>),*/
        /*                  "Invalid matrix size");*/
        /*}*/

        Matrix(const Matrix& other) : matrix{other.matrix} {}
        Matrix(Matrix&& other) noexcept : matrix{std::move(other.matrix)} {}

        Matrix& operator=(const Matrix& other) {
            if (this == &other) return *this;
            matrix = other.matrix;
            return *this;
        }

        vector<ColumnVector<number, m>> colToVectorSet() const {
            vector<ColumnVector<number, m>> res{};
            for (size_t i{}; i < n; i++) {
                Vector<number, m> vec;
                for (size_t j{}; j < m; j++) vec.at(j) = matrix.at(j).at(i);
                res.push_back(vec.T());
            }
            return res;
        }

        vector<Vector<number, m>> rowToVectorSet() const {
            vector<Vector<number, m>> res;
            for (const auto& row : matrix) res.push_back(row);
            return res;
        }

        Matrix operator*(ColumnVector<number, n> vec) const {
            Matrix<number, m, n> res{};
            for (size_t i{}; i < n; i++) {
                const auto& mult = vec.at(i).at(0);
                for (size_t j{}; j < m; j++) {
                    res.at(j).at(i) = mult * this->matrix.at(j).at(i);
                }
            }
            return res;
        }

        Matrix operator*(Vector<number, n> vec) const {
            Matrix<number, m, n> res{};
            for (size_t i{}; i < n; i++) {
                const auto& mult = vec.at(i);
                for (size_t j{}; j < m; j++) res.at(j).at(i) = mult * this->matrix.at(j).at(i);
            }
            return res;
        }

        template <size_t nOther>
        Matrix<number, m, nOther> operator*(const Matrix<number, n, nOther> other) const {
            return multByDef(other);
        }

        Matrix& operator=(Matrix&& other) noexcept {
            matrix = std::move(other.matrix);
            return *this;
        }

        ~Matrix() = default;

        friend std::ostream& operator<<(std::ostream& os, const Matrix<number, m, n>& system) {
            for (const auto row : system) os << row;
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
        static Matrix<num, mN, nN> fromColVectorSet(const vector<ColumnVector<num, mN>>& vecSet) {
            Matrix<num, mN, nN> res{};
            for (size_t i{}; i < nN; i++) {
                const auto& colVec{vecSet.at(i)};
                for (size_t j{}; j < mN; j++) res.at(j).at(i) = colVec.at(j).at(0);
            }
            return res;
        }

       private:
        template <size_t nOther>
        Matrix<number, m, nOther> multByDef(const Matrix<number, n, nOther>& other) const {
            auto otherColVecSet{other.colToVectorSet()};
            vector<ColumnVector<number, m>> res{};
            for (size_t i{}; i < nOther; i++) {
                const auto& col{otherColVecSet.at(i)};
                auto multRes = *this * col;
                Vector<number, m> sumRes{};
                for (auto& col : multRes.colToVectorSet()) {
                    auto colTrans = std::get<Vector<number, m>>(col.T());
                    sumRes = sumRes + colTrans;
                }
                res.push_back(sumRes.T());
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

}  // namespace mlinalg::structures
