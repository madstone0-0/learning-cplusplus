#include <MLinalg.hpp>
#include <iostream>

using std::cout;

template <Number number, size_t m, size_t n>
auto printSol(mlinalg::LinearSystem<number, m, n> system) {
    using namespace mlinalg;
    auto sol{findSolutions(system)};
    cout << "For the system:\n" << system << "\nThe solutions are:";
    if (!sol.has_value()) {
        cout << " The system is inconsistent\n\n";
        return;
    }
    cout << sol << '\n';
    /*cout << "[";*/
    /*auto& solVal = sol.value();*/
    /*for (size_t i{}; i < solVal.size(); i++)*/
    /*    if (i != solVal.size() - 1) {*/
    /*        if (solVal.at(i).has_value())*/
    /*            cout << solVal.at(i).value() << ", ";*/
    /*        else*/
    /*            cout << "None, ";*/
    /**/
    /*    } else {*/
    /*        if (solVal.at(i).has_value())*/
    /*            cout << solVal.at(i).value();*/
    /*        else*/
    /*            cout << "None";*/
    /*    }*/
    /*cout << " ]\n\n";*/
}

int main() {
    using namespace mlinalg;
    auto sys1 = LinearSystem<double, 3, 4>{{{1, -2, 1, 0}, {0, 2, -8, 8}, {5, 0, -5, 10}}};
    LinearSystem<double, 3, 5> sys2{{{
                                         3,
                                         0,
                                         -1,
                                         0,
                                         0,
                                     },
                                     {
                                         8,
                                         0,
                                         0,
                                         -2,
                                         0,
                                     },
                                     {
                                         8,
                                         2,
                                         -2,
                                         1,
                                         0,
                                     }}};
    LinearSystem<double, 3, 4> sys9 = {{2, 0, -6, -8}, {0, 1, 2, 3}, {3, 6, -2, -4}};
    auto sys4 = LinearSystem<double, 3, 3>{{2, -1, 0}, {-3, 1, -1}, {2, -3, 4}};
    LinearSystem<double, 3, 5> sys3 = {
        {0.35, -0.1, -0.25, 0, 0},
        {-0.3, 0.9, -0.35, -0.25, 0},
        {-0.3, -0.15, 0.85, -0.4, 0},
        {-0.2, -0.1, -0.3, 0.6, 0},
    };

    Matrix<double, 4, 3> sys5{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};

    Matrix<double, 2, 2> mult1{{2, 3}, {1, -5}};
    Matrix<double, 2, 3> mult2{{4, 3, 6}, {1, -2, 3}};

    LinearSystem<double, 3, 3> sys6{
        {27.6, 30.2, 162},
        {3100, 6400, 23610},
        {
            250,
            360,
            1623,
        },

    };

    LinearSystem<double, 3, 4> sys7{
        {
            1,
            3,
            -5,
            0,
        },
        {
            1,
            4,
            -8,
            0,
        },
        {
            -3,
            -7,
            9,
            0,
        },
    };

    printSol(sys1);
    printSol(sys2);
    printSol(sys9);
    printSol(sys4);
    printSol(sys3);
    cout << sys1 * Row<double, 4>{1, 2, 3, 4}.T() << "\n";
    cout << mult1 * mult2 << "\n";
    cout << sys4 * sys4 * sys4 * sys4 << "\n";
    cout << sys1.T() << "\n";
    printSol(sys6);
    printSol(sys7);
    return 0;
}
