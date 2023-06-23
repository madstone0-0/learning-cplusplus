#include <cstdio>

/* Goto Statements */
int main() {
    goto silent_bob;
luke:
    printf("I'm not afraid.\n");
    goto yoda;
silent_bob:
    goto luke;
yoda:
    printf("You will be\n");
    return 0;
}
/* Goto Statements */
