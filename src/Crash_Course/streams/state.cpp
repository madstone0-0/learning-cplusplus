#include <iostream>
#include <string>

/*Stream State*/

int main() {
    using namespace std;
    cin.exceptions(istream::badbit);

    string word;
    size_t count{};

    try {
        while (cin >> word) count++;

        cout << "Discovered " << count << " words.\n";

    } catch (const std::exception& e) {
        cerr << "Error occured reading from stdin: " << e.what();
    }

    return 0;
}

/*Stream State*/
