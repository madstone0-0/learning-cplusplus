#include <iomanip>
#include <ios>
#include <iostream>
#include <string>

using namespace std;
/* Manipulators */

// int main() {
//     string word;
//     size_t count{};
//
//     while (cin >> word) count++;
//
//     cout << "Discovered " << count << " words." << endl;
//
//     return 0;
// }

int main() {
    cout << "Boolean:\n"
         << "AplhaNumeric: " << boolalpha << true << "\nNumeric: " << noboolalpha << false << "\n\n";
    cout << "Ints:"
         << "\nBase 8: " << oct << 100 << "\nBase 10: " << dec << 100 << "\nBase 16: " << hex << 100 << "\n\n";
    cout << "Floats:"
         << "\nFixed: " << fixed << 3.1415926536 << "\nScientific: " << scientific << 3.1415926536 << "\n\n";
    cout << "\nAlways eliminate " << 3735929054;
    cout << setw(4) << "\n" << 0x1 << "\n" << 0x10 << "\n" << 0x100 << "\n" << 0x1000 << endl;

    return 0;
}

/* Manipulators */
