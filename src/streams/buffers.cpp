#include <iostream>
#include <iterator>

/* Output Stream Buffer iterators */
using namespace std;
const string path{"/home/mads/projects/C++/Learning/src/streams/"};

// int main() {
//     ostreambuf_iterator<char> itr{cout.rdbuf()};
//     *itr = 'H';
//     ++itr;
//     *itr = 'i';
//     itr++;
//     *itr = '\n';
//
//     return 0;
// }

/*  Output Stream Buffer iterators  */

/* Input Sream Buffer iterators */

// int main() {
//     istreambuf_iterator<char> cin_itr{cin}, end{};
//     cout << "What is your name?\n";
//     const string name{cin_itr, end};
//     cout << "\nGoodbye " << name << endl;
//
//     return 0;
// }

/* Input Sream Buffer iterators */

/* Random Access */

#include <exception>
#include <fstream>

ifstream open(const char* path, ios_base::openmode mode = ios_base::in) {
    ifstream file{path, mode};
    if (!file.is_open()) {
        string err{"Unable to open file "};
        err.append(path);
        throw runtime_error(err);
    }
    file.exceptions(ios::badbit);
    return file;
}

int main() {
    const string file_path = path + "introspection.txt";
    try {
        auto file = open(file_path.c_str());
        cout << "Contents: " << file.rdbuf() << endl;
        file.seekg(0);
        cout << "Contents after seekg(0): " << file.rdbuf() << endl;
        file.seekg(-5, ios_base::end);
        cout << "tellg() after seekg(-4, ios_base::end): " << file.tellg() << endl;
        cout << "Contents after seekg(-4, ios_base::end): " << file.rdbuf() << endl;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}

/* Random Access */
