#include <MLinalg.hpp>
#include <boost/rational.hpp>
#include <iostream>

#include "Structures.hpp"

using boost::rational;
using namespace std;
using namespace mlinalg::structures;

int main() {
    {
        auto e1 = Vector<double, 2>{1, 0};
        auto e2 = Vector<double, 2>{0, 1};
        auto A = Matrix<double, 3, 3>{{1, -2, 1}, {0, 2, -8}, {5, 0, -5}};
        auto b = Vector<double, 3>{1, 0, -1};
        cout << "Vectors\n\n";
        cout << e1 + e2 << '\n';
        cout << e2 + e1 << '\n';
        cout << A << '\n';
        cout << (A * b).T() << '\n';
        cout << e1.T() * e2 << '\n';
        cout << e1 * e2 << '\n';
        cout << e2 - e1 << '\n';
        cout << e1.T() << '\n';
        cout << e2.T() << '\n';
        e1 = e2.T();
        cout << e1 << '\n';
        cout << e1.dist(e2) << '\n';
        cout << e1.dot(e1) << '\n';
        cout << (e1.T() * e1).at(0) << '\n';
    }
    {
        cout << "\nMatrices\n\n";
        auto A = Matrix<double, 2, 2>{{5, 1}, {3, -2}};
        auto B = Matrix<double, 2, 2>{{2, 0}, {4, 3}};
        cout << A << '\n';
        cout << A * B << '\n';
        cout << mlinalg::I<double, 2>() << '\n';
        auto Aaug = A.augment(mlinalg::I<double, 2>());
        cout << Aaug << '\n';
        cout << mlinalg::rref(Aaug) << '\n';
    }
    return 0;
}
