#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <string>
#include <type_traits>

using namespace std;
using boost::multiprecision::cpp_int;

template <typename T>
concept Numbers = requires(T a, T b) {
    a += b;
    a++;
    b++;
    b += a;
};

template <Numbers T>
class Progression {
   public:
    Progression(T f = 0) : first{f}, cur{f} {}

    void printProgression(const int& n) {
        cout << first << ' ';
        for (int i{2}; i <= n; i++) {
            cout << nextValue() << ' ';
        }
        cout << '\n';
    }

    virtual ~Progression() = default;

   protected:
    T cur;
    T first;

    virtual T firstValue() {
        cur = first;
        return cur;
    }
    virtual T nextValue() = 0;
};

template <Numbers T>
class ArithmeticProgression : public Progression<T> {
   public:
    ArithmeticProgression(T f = 0, T inc = 1) : Progression<T>{f}, inc{inc} {}

   protected:
    T inc;

    T nextValue() override {
        this->cur += inc;
        return this->cur;
    }
};

template <Numbers T>
class GeometricProgression : public Progression<T> {
   public:
    GeometricProgression(T f = 1, T r = 2) : Progression<T>{f}, ratio{r} {}

   protected:
    T ratio;

    T nextValue() override {
        this->cur *= ratio;
        return this->cur;
    }
};

int main(int argc, char const* argv[]) {
    ArithmeticProgression arth{ArithmeticProgression<double>{0, 10}};
    GeometricProgression geo{GeometricProgression<cpp_int>{1, 10}};
    arth.printProgression(10);
    geo.printProgression(1000);
    return 0;
}
