#include <algorithm>
#include <boost/algorithm/searching/boyer_moore.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/program_options.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace op = boost::program_options;
namespace fs = std::filesystem;

using std::cout, std::endl, std::string, std::cerr, std::vector, std::regex, std::smatch, std::ifstream,
    std::stringstream, std::regex_search, std::string_view;

struct Result {
    bool status;
    string line;
};

ifstream open(const fs::path path, std::ios_base::openmode mode = std::ios_base::in) {
    ifstream file{path, mode};
    if (!file.is_open()) {
        string err{"Unable to open file "};
        err.append(path);
        throw std::runtime_error(err);
    }
    file.exceptions(std::ios::badbit);
    return file;
}

string readFile(ifstream& file) {
    stringstream contentSS{stringstream{} << file.rdbuf()};
    return contentSS.str();
}

bool searchFile(fs::path item, string pattern) {
    if (!fs::is_regular_file(item)) return false;

    boost::algorithm::boyer_moore bm{pattern.begin(), pattern.end()};

    auto contentStream = open(item);
    string contentString = readFile(contentStream);
    std::replace(contentString.begin(), contentString.end(), '\n', ' ');

    const auto [resBeg, resEnd] = bm(contentString);
    return resBeg != resEnd;
}

bool searchFile(fs::path item, regex pattern) {
    if (!fs::is_regular_file(item)) return false;
    // smatch results;

    auto contentStream = open(item);
    string contentString = readFile(contentStream);
    std::replace(contentString.begin(), contentString.end(), '\n', ' ');
    const auto result = regex_search(contentString, pattern);
    return result;
}

vector<fs::path> searchDir(fs::path dir, string pattern) {
    // smatch results;
    vector<fs::path> paths;

    for (const auto& item : fs::directory_iterator{dir}) {
        if (searchFile(item, pattern)) {
            paths.push_back(item.path());
        }
    }
    return paths;
}

vector<fs::path> searchDir(fs::path dir, regex pattern) {
    // smatch results;
    vector<fs::path> paths;

    for (const auto& item : fs::directory_iterator{dir}) {
        if (searchFile(item, pattern)) {
            paths.push_back(item.path());
        }
    }
    return paths;
}

vector<fs::path> searchDirRecursively(fs::path dir, string pattern) {
    vector<fs::path> paths;

    for (const auto& item : fs::recursive_directory_iterator{dir}) {
        if (searchFile(item, pattern)) {
            paths.push_back(item.path());
        }
    }
    return paths;
}

vector<fs::path> searchDirRecursively(fs::path dir, regex pattern) {
    vector<fs::path> paths;

    for (const auto& item : fs::recursive_directory_iterator{dir}) {
        if (searchFile(item, pattern)) {
            paths.push_back(item.path());
        }
    }
    return paths;
}

int main(int argc, char** argv) {
    using namespace op;
    bool isRecursive{}, isHelp{}, isRegex{};

    options_description desc{"mgrep [options] pattern path1 path2..."};
    desc.add_options()("help,h", bool_switch(&isHelp), "display help")("threads,t", value<int>()->default_value(4),
                                                                       "number of threads to use")(
        "recursive,r", bool_switch(&isRecursive), "search subddirectories recursively")(
        "regex", bool_switch(&isRegex), "parse pattern as regex")("pattern", value<string>(), "search pattern")(
        "path", value<std::vector<string>>(), "paths to search");
    positional_options_description positional;
    positional.add("pattern", 1);
    positional.add("path", -1);

    command_line_parser parser{argc, argv};
    parser.options(desc);
    parser.positional(positional);

    variables_map vm;
    try {
        auto parsedResult = parser.run();
        store(parsedResult, vm);
        notify(vm);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
        return -1;
    }

    if (isHelp) {
        cout << desc;
        return 0;
    };

    if (vm["pattern"].empty()) {
        std::cerr << "You must provide a pattern" << endl;
        return -1;
    }

    if (vm["path"].empty()) {
        std::cerr << "You must provide at least one path" << endl;
        return -1;
    }

    const auto threads = vm["threads"].as<int>();
    // const auto& pattern = boost::to_lower_copy(vm["pattern"].as<string>());
    const auto& pattern = vm["pattern"].as<string>();
    const auto& path = vm["path"].as<std::vector<string>>();

    regex re;
    try {
        re = regex{pattern};
    } catch (const std::regex_error&) {
        if (isRegex) {
            cerr << "Invalid regex" << endl;
            return -1;
        }
    }

    vector<fs::path> paths;
    try {
        for (const auto& p : path) {
            if (!isRegex) {
                if (fs::is_directory(p) && !isRecursive) {
                    paths = searchDir(p, pattern);
                } else if (fs::is_directory(p) && isRecursive) {
                    paths = searchDirRecursively(p, pattern);
                } else if (fs::is_regular_file(p)) {
                    if (!searchFile(p, pattern)) continue;
                    paths.push_back(p);
                } else {
                    cerr << p << " cannot be searched." << endl;
                    return -1;
                }
            } else {
                if (fs::is_directory(p) && !isRecursive) {
                    paths = searchDir(p, re);
                } else if (fs::is_directory(p) && isRecursive) {
                    paths = searchDirRecursively(p, re);
                } else if (fs::is_regular_file(p)) {
                    if (!searchFile(p, re)) continue;
                    paths.push_back(p);
                } else {
                    cerr << p << " cannot be searched." << endl;
                    return -1;
                }
            }
        }

    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }

    cout << "Pattern \"" << pattern << "\" found in the following files:\n";
    for (const auto& path : paths) cout << "\t" << path.string() << "\n";
    cout << "\n";

    cout << "OK" << endl;

    return 0;
}
