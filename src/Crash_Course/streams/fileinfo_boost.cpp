#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <exception>
#include <iomanip>
#include <ios>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace io = boost::iostreams;
using std::cerr, std::cout, std::string, std::ios_base, std::endl, std::string_view, std::stringstream,
    boost::algorithm::split;

auto open(const char* path) {
    io::stream_buffer<io::file_sink> file(path);
    if (!file.is_open()) {
        string err{"Cannot open file "};
        err.append(path);
        throw std::runtime_error(err);
    }
    return file;
}

int main(int argc, char* argv[]) { return 0; }
