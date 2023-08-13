#include <cstdio>
#include <iostream>
#define MESSAGE "LOL"
#define SAY_LOL_WITH(fn) fn("LOL")

using std::cout, std::endl;

int main() {
    cout << MESSAGE << endl;
    SAY_LOL_WITH(printf);
    return 0;
}
