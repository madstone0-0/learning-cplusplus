#include <cstdio>
#include <iostream>

struct Tracer {
    Tracer(const char* name);
    ~Tracer();

   private:
    const char* name;
};
