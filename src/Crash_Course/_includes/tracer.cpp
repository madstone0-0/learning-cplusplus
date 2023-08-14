#include "tracer.hpp"

using std::cout;

Tracer::Tracer(const char* name) : name{name} {
    cout << "(" << name << ")"
         << " constructed\n";
};

Tracer::~Tracer() {
    cout << "(" << name << ")"
         << " destructed\n";
};
