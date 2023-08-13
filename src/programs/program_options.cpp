#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace op = boost::program_options;

using std::cout, std::endl, std::string;

int main(int argc, char** argv) {
    using namespace op;
    bool isRecursive{}, isHelp{};

    options_description desc{"mgrep [options] pattern path1 path2..."};
    desc.add_options()("help,h", bool_switch(&isHelp), "display help")("threads,t", value<int>()->default_value(4),
                                                                       "number of threads to use")(
        "recursive,r", bool_switch(&isRecursive), "search subddirectories recursively")(
        "pattern", value<string>(), "search pattern")("path", value<std::vector<string>>(), "paths to search");
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
    const auto& pattern = vm["pattern"].as<string>();
    const auto& path = vm["path"].as<std::vector<string>>();

    cout << "OK" << endl;

    return 0;
}
