#include <cerrno>
#include <cstdio>
#include <memory>

using FileGuard = std::unique_ptr<FILE, int (*)(FILE*)>;

void sayHello(FileGuard file) { fprintf(file.get(), "HELLO DAVE"); }

int main() {
    auto file = fopen("./HAL9000", "w");
    if (!file) return errno;

    FileGuard fileGuard{file, fclose};
    sayHello(std::move(fileGuard));

    return 0;
}
