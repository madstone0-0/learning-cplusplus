#include <cerrno>
#include <cstdio>
#include <memory>

using FileGuard = std::shared_ptr<FILE>;  // Shared Pointer

void sayHello(FileGuard file) { fprintf(file.get(), "HELLO DAVE\n"); }

struct Hal {
    Hal(std::shared_ptr<FILE> file) : file{file} {}

    ~Hal() { fprintf(file.get(), "Stop, Dave.\n"); }

    void writeStatus() { fprintf(file.get(), "I'm completely operational\n"); }

    std::shared_ptr<FILE> file;
};

int main() {
    auto file = fopen("./HAL9000", "w");
    if (!file) return errno;

    FileGuard fileGuard{file, fclose};
    // sayHello(fileGuard);
    // sayHello(fileGuard);
    Hal hal1{fileGuard};
    Hal hal2{fileGuard};
    Hal hal3{fileGuard};
    Hal hal4{fileGuard};

    hal1.writeStatus();
    hal2.writeStatus();
    hal3.writeStatus();
    hal4.writeStatus();

    return 0;
}
