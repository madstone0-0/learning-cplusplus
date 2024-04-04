#include <iostream>

using namespace std;

void squareByRef(int& num);
int squareByVal(int num);

int main() {
    auto x{2};
    auto z{4};

    cout << "x = " << x << '\n';
    cout << "z = " << z << '\n';

    cout << "squareByVal(x) = " << squareByVal(x) << '\n';

    squareByRef(z);
    cout << "z after squareByRef = " << z << '\n';

    cout << "x = " << x << '\n';
    cout << "z = " << z << '\n';
}

int squareByVal(int num) { return num *= num; }

void squareByRef(int& num) { num *= num; }
