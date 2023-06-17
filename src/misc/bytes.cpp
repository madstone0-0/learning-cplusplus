#include <iostream>
#include <string>

using std::cout, std::cin, std::string;

int main(int argc, char *argv[]) {
    cout << "Baller\n";
    for (auto i = 1; i < argc; i++) {
        cout << argv[i] << '\n';
    }

    return 0;
}
