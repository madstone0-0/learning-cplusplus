#include <cstdio>
#include <cstring>
#include <set>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Need arguments\n");
        return 1;
    }
    auto alpha = [](const char* a, const char* b) { return strcmp(a, b) < 0; };

    std::set<const char*, decltype(alpha)> alphaNumeric(argv + 1, argv + argc);

    for (const auto i : alphaNumeric) {
        printf("%s ", i);
    }
    printf("\n");

    return 0;
}
