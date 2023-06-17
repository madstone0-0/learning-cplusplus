#include <cstdio>

int abs(int x) {
    if (x < 0) {
        return x * -1;
    } else {
        return x;
    }
}

int main() {
    int x = -5;
    int y = 5;
    printf("abs(%d) = %d\n", x, abs(x));
    printf("abs(%d) = %d\n", y, abs(y));
    return 0;
}
