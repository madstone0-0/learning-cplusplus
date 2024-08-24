#include <MLinalg.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

using std::cout, std::endl, std::cin, std::vector, std::string, std::stringstream;
namespace op = boost::program_options;
namespace ba = boost::algorithm;

template <Number number, size_t m, size_t n>
auto printSol(mlinalg::LinearSystem<number, m, n> system) {
    using namespace mlinalg;
    auto sol{findSolutions(system)};
    cout << "For the system:\n" << system << "\nThe solutions are: ";
    if (!sol.has_value()) {
        cout << "The system is inconsistent\n\n";
        return;
    }
    cout << sol << '\n';
}

template <Number number, size_t m, size_t n>
auto printSol(mlinalg::Matrix<number, m, n> A, mlinalg::Vector<number, m> b) {
    using namespace mlinalg;
    auto sol{findSolutions(A, b)};
    cout << "For the system:\n" << A.augment(b) << "\nThe solutions are: ";
    if (!sol.has_value()) {
        cout << "The system is inconsistent\n\n";
        return;
    }
    cout << sol << '\n';
}

int main(int argc, char *argv[]) { return 0; }
