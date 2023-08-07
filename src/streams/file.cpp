// #define CATCH_CONFIG_MAIN
// #include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <limits>

using namespace std;
const string path{"/home/mads/projects/C++/Learning/src/streams/"};

/* Output */

// int main() {
//     ofstream file{path + "time_adventure.txt", ios::out | ios::trunc};
//     file << "Time is an illusion," << endl;
//     file << "That helps things make sense." << endl;
//
//     ofstream lunchtime{path + "lunchtime.txt", ios::out | ios::app};
//     lunchtime << "Time is an illusion." << endl;
//     lunchtime << "Lunch time," << 2 << "x so" << endl;
//
//     return 0;
// }

/* Output */

/* Input */

// int main() {
//     ifstream file{path + "numbers.txt"};
//     auto max = numeric_limits<int>::min();
//     int value;
//     while (file >> value) {
//         max = max < value ? value : max;
//     }
//     cout << "Maximum was found was " << max << endl;
//
//     return 0;
// }

/* Input */

/* Failure */

ifstream open(const char* path, ios_base::openmode mode = ios_base::in) {
    ifstream file{path, mode};
    if (!file.is_open()) {
        string err{"Unable to open file "};
        err.append(path);
        throw runtime_error{err};
    }
    file.exceptions(ifstream::badbit);
    return file;
}

#include <sstream>
int main() {
    const string file_path = path + "time_adventure.txt";

    try {
        auto time_adventure = open(file_path.c_str());
        cout << time_adventure.rdbuf();

    } catch (const runtime_error& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}

/* Failure */
