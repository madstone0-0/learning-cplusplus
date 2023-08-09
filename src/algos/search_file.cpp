#include <boost/algorithm/searching/boyer_moore.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

using std::cout, std::cerr, std::string, std::string_view, std::endl, std::ifstream, std::ios_base, std::runtime_error,
    std::ios, std::stringstream, boost::algorithm::boyer_moore_search, boost::to_lower;

ifstream open(const fs::path path, ios_base::openmode mode = ios_base::in) {
    ifstream file{path, mode};
    if (!file.is_open()) {
        string err{"Unable to open file "};
        err.append(path);
        throw runtime_error(err);
    }
    file.exceptions(ios::badbit);
    return file;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: search_file FILENAME TEXT" << endl;
        return -1;
    }

    try {
        const string path = fs::path{argv[1]};
        string text{argv[2]};

        auto file = open(path);
        const stringstream file_contents = stringstream{} << file.rdbuf();

        to_lower(text);
        const auto [first, second] = boyer_moore_search(file_contents.str().cbegin(), file_contents.str().cend(), text);

        if (!(first == file_contents.str().cend())) {
            cout << "Text: " << text << " exists in file " << path << endl;
        } else {
            cout << "Text: " << text << " does not exist in file " << path << endl;
        }

    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
