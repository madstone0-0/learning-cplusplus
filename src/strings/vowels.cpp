#include <boost/algorithm/string/case_conv.hpp>
#include <cstdio>
#include <regex>
#include <string>
#include <string_view>

using std::regex, std::string, std::smatch, std::regex_match;
int main(int argc, char* argv[]) {
    if (argc == 1 || argc > 2) {
        printf("Invalid number of arguments.\n");
        return 1;
    }

    string input = argv[1];
    regex vowels{R"(([aeiouAEIOU]+))"};
    smatch res;

    size_t count{};
    for (const auto& c : input) {
        string s{c};
        if (regex_match(s, res, vowels)) count++;
    }

    if (count == 0) {
        printf("No vowels found\n");
        return 0;
    }

    printf("Number of vowels: %zu\n", count);

    return 0;
}
