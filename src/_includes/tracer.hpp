#ifndef TRACER_INCLUDE
#define TRACER_INCLUDE

#include <cstdio>
#include <iostream>

struct Tracer {
    Tracer(const char* name);
    ~Tracer();

   private:
    const char* name;
};

#endif
