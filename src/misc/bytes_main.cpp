#include <algorithm>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/program_options.hpp>
#include <exception>
#include <iostream>

#include "bytes.hpp"
using std::cout, std::string, boost::to_upper, std::cerr, std::endl;

int main(int argc, char* argv[]) {
    using namespace Bytes;
    using namespace boost::program_options;
    bool isHelp{};

    options_description description{"bytes [options] value"};
    description.add_options()("help,h", bool_switch(&isHelp), "display help")(
        "from", value<string>(), "the value's unit")("to", value<string>(), "the unit to convert to")(
        "value", value<double>(), "value to convert");

    positional_options_description positional;
    positional.add("value", 1);

    command_line_parser parser{argc, argv};
    parser.options(description);
    parser.positional(positional);

    variables_map vm;
    try {
        auto parsedResult = parser.run();
        store(parsedResult, vm);
        notify(vm);
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }

    if (isHelp || argc == 1) {
        cout << description;
        return 0;
    }

    if (vm["from"].empty()) {
        cerr << "You must provide an initial unit" << endl;
        return -1;
    }

    if (vm["to"].empty()) {
        cerr << "You must provide a result unit" << endl;
        return -1;
    }

    if (vm["value"].empty()) {
        cerr << "You must provide a value" << endl;
        return -1;
    }

    const auto value = vm["value"].as<double>();
    auto unit = vm["from"].as<string>();
    auto toUnit = vm["to"].as<string>();

    to_upper(unit);
    to_upper(toUnit);

    try {
        Byte result = convert(value, unit, toUnit);
        printf("%g %s = %g %s\n", value, unit.c_str(), result.value, result.unit.c_str());
    } catch (const std::exception& e) {
        printf("Execption: %s\n", e.what());
        return 1;
    }

    return 0;
}
