#include <cstdio>

#include "tracer.cpp"

int main() {
    Tracer main{"main"};

    {
        printf("Block a\n");
        Tracer a1{"a1"};
        Tracer a2{"a2"};
    }

    {
        printf("Block b\n");
        Tracer b1{"b1"};
        Tracer b2{"b2"};
    }

    return 0;
}
