#include <cstdio>

/* Tracing the object life cycle */
struct Tracer {
    Tracer(const char* name) : name{name} { printf("%s constructed\n", name); }

    ~Tracer() { printf("%s destructed\n", name); }

   private:
    const char* const name;
};

static Tracer t1{"Static"};
thread_local Tracer t2{"Thread Local"};

int main() {
    const auto t2_ptr = &t2;
    printf("A\n");
    Tracer t3{"Automatic"};
    printf("B\n");
    const auto* t4 = new Tracer{"Dynamic"};
    printf("C\n");
    return 0;
}
