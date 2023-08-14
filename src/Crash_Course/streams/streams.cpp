// 
// #include <catch2/catch_all.hpp>
#include <bitset>
#include <iostream>
#include <string>

/* Formatted Operations */

// int main() {
//     using namespace std;
//
//     bitset<8> s{"01110011"};
//     string str{"Crying zeros and I'm hearing "};
//     size_t num{111};
//
//     cout << s << '\n' << str << num << "s\n";
//     return 0;
// }

int main() {
    using namespace std;
    double x, y;
    cout << "X: ";
    cin >> x;

    cout << "Y: ";
    cin >> y;

    string op;
    cout << "Operation: ";
    cin >> op;

    if (op == "+")
        cout << x + y;
    else if (op == "-")
        cout << x - y;
    else if (op == "*")
        cout << x * y;
    else if (op == "/")
        cout << x / y;
    else
        cout << "Unknown operation " << op;

    return 0;
}

/* Formatted Operations */
