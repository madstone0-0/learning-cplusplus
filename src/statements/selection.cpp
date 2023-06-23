#include <cstdio>
#include <stdexcept>
#include <type_traits>

/* If Statements */

// Init Statements and If
struct TextFile {
    bool success;
    const char* contents;
    size_t n_bytes;
};

TextFile readTextFile(const char* path) {
    const static char contents[]{"Spent long time rolling down in the basement, just rolling with the riff-raff\n"};
    return TextFile{true, contents, sizeof(contents)};
}

// int main() {
//     if (const auto [status, contents, length] = readTextFile("BALLING.txt"); status) {
//         printf("Bytes: %zu\nContents: %s", length, contents);
//     } else {
//         printf("Could not read BALLLING.txt");
//     }
//
//     return 0;
// }

// Constexpr If Statements
template <typename T>
auto valueOf(T x) {
    if constexpr (std::is_pointer<T>::value) {
        if (!x) throw std::runtime_error{"Null pointer deference"};
        return *x;
    } else {
        return x;
    }
}

// int main() {
//     unsigned long level{8998};
//     auto level_ptr = &level;
//     auto& level_ref = level;
//     printf("Power level = %lu\n", valueOf(level_ptr));
//     ++*level_ptr;
//     printf("Power level = %lu\n", valueOf(level_ref));
//     ++level_ref;
//     printf("It's over %lu!\n", valueOf(level++));
//     try {
//         level_ptr = nullptr;
//         valueOf(level_ptr);
//     } catch (const std::exception& e) {
//         printf("Exception: %s\n", e.what());
//     }
// }

/* If Statements */

/* Switch Statements */

enum class Color { Mauve, Pink, Russet };

struct Result {
    const char* name;
    Color color;
};

Result observeShrub(const char* name) { return Result{name, Color::Russet}; }

int main() {
    const char* description;
    switch (const auto result = observeShrub("Zaphod"); result.color) {
        case Color::Mauve: {
            description = "mauvey shade of pinky russet";
            break;
        }
        case Color::Pink: {
            description = "pinky shade of mauvey russet";
            break;
        }
        case Color::Russet: {
            description = "russety shade of pinky mauve";
            break;
        }
        default: {
            description = "enigmatic shade of whitish black";
        }
    }

    printf(
        "The other Shaltanac's joopleberry shrub is "
        "always a more %s.\n",
        description);
}

/* Switch Statements */
