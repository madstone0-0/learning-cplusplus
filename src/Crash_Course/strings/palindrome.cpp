#include <boost/algorithm/string/case_conv.hpp>
#include <cstdio>
#include <string>
#include <string_view>

using std::string, std::string_view, boost::algorithm::to_lower_copy, boost::algorithm::to_lower;
bool isPalindrome(const string& x) {
    string reversed{x.rbegin(), x.rend()};
    to_lower(reversed);
    return reversed == to_lower_copy(x);
}

int main(int argc, char** argv) {
    if (argc == 1 || argc > 2) {
        printf("Invalid number of arguments.\n");
        return 1;
    }

    string input = argv[1];
    string res{};

    if (isPalindrome(input)) {
        res = "a palindrome.";
    } else {
        res = "not a palindrome.";
    }

    printf("Your input is %s\n", res.c_str());
}
