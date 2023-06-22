#include <iostream>
#include <string>

using std::cout, std::cin, std::string;

int main(int argc, char *argv[]) {
    string input{""};
    if (argc == 0) {
        cout << "Please enter value:\n";
        cin >> input;
    } else {
        input = argv[1];
        input.append(argv[2]);
    }

    cout << input << "\n";

    return 0;
}
