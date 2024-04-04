#include <boost/type_index.hpp>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

long div2(long x) {
    long n{};
    while (x > 2) {
        x /= 2;
        n++;
    }
    return n;
}

template <typename T>
concept Scalar = requires(T a, T b) {
    a + b;
    a* b;
    a - b;
};

using std::vector;

template <Scalar S>
class Vector {
   public:
    Vector(std::initializer_list<S> data) : _vec{data}, size{static_cast<int>(_vec.size())} {}
    explicit Vector(const int& size) : size{size}, _vec(size, S{0}) {}

    Vector operator+(const Vector& other) const {
        if (other._vec.size() != _vec.size()) throw std::runtime_error{"Vectors must be the same length"};

        Vector<S> res(size);
        for (size_t i{}; i < size; i++) {
            res.at(i) = _vec.at(i) + other._vec.at(i);
        }

        return res;
    }

    Vector(const Vector& other) : _vec{other._vec}, size{other.size} {}

    Vector& operator=(const Vector& other) {
        if (&other == this) return *this;
        size = other.size;
        _vec = other._vec;
        return *this;
    }

    Vector(Vector&& other) noexcept : _vec{std::move(other._vec)}, size{std::move(other.size)} {}

    Vector& operator=(Vector&& other) noexcept {
        if (this == &other) return *this;
        size = std::move(other.size);
        _vec = std::move(other._vec);
        return *this;
    }

    ~Vector() = default;

    Vector operator-(const Vector& other) const {
        if (other._vec.size() != _vec.size()) throw std::runtime_error{"Vectors must be the same length"};

        vector<S> res(size);
        for (size_t i{}; i < size; i++) {
            res.at(i) = _vec.at(i) - other._vec.at(i);
        }
    }

    Vector operator*(const Vector& other) const {
        if (other._vec.size() != _vec.size()) throw std::runtime_error{"Vectors must be the same length"};
        Vector<S> res(size);

        for (size_t i{}; i < size; i++) {
            res.at(i) = _vec.at(i) * other._vec.at(i);
        }

        return res;
    }

    Vector operator*(const S& scalar) const {
        Vector<S> res(size);
        for (size_t i{}; i < size; i++) {
            res.at(i) = _vec.at(i) * scalar;
        }
        return res;
    }

    [[nodiscard]] S dotProd(const Vector& other) const {
        if (other._vec.size() != _vec.size()) throw std::runtime_error{"Vectors must be the same length"};

        S res{0};
        for (size_t i{}; i < size; i++) {
            res = res + _vec.at(i) * other._vec.at(i);
        }

        return res;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector<S>& vec) {
        using boost::typeindex::type_id_with_cvr;
        os << "Vector(" << type_id_with_cvr<S>() << "){ ";
        for (const auto& i : vec._vec) os << i << ", ";
        os << "}";
        return os;
    }

   private:
    vector<S> _vec;
    int size;

    S& at(const size_t& n) { return _vec.at(n); }
};

class Complex {
   public:
    Complex(double real = 0.0, double img = 0.0) : real{real}, img{img} {}

    Complex operator+(const Complex& other) const { return {real + other.real, img + other.img}; }

    Complex operator-(const Complex& other) const { return {real - other.real, img - other.img}; }

    Complex operator*(const Complex& other) const {
        return {real * other.real - img * other.img, real * other.img + img * other.real};
    }

    friend std::ostream& operator<<(std::ostream& s, const Complex& comp) {
        std::string sign{comp.img < 0 ? " - " : " + "};
        s << comp.real << sign << std::abs(comp.img) << 'i';
        return s;
    }

   private:
    double real;
    double img;
};

int main() {
    using namespace std;
    cout << div2(50000) << '\n';
    Vector<int> intVec{1, 2, 4, 5};
    Vector<int> intVec2{2, 3, 2, 1};
    cout << intVec << '\n';
    intVec = intVec * 10;
    cout << intVec << '\n';
    intVec = intVec + intVec2;
    cout << intVec << '\n';

    Vector<Complex> comVec{{1, 3}, {2, -4}, {2, 3}};
    cout << comVec << '\n';
    comVec = comVec * 10;
    cout << comVec << '\n';
    cout << comVec.dotProd(comVec) << '\n';

    return 0;
}
