#include <cstdio>
#include <stdexcept>

/* Namespaces */

namespace SHADOW {
    namespace WIZARD {}
}  // namespace SHADOW

namespace MONEY::GANG {
    enum class Color { Bood, Svart, Vit, Gront };

    struct Realm {
        const char* name;
        Color color;
    };

    bool isDarker(const Realm& realm) { return realm.color == Color::Svart; }
}  // namespace MONEY::GANG

// using namespace MONEY::GANG;
// int main() {
//     const auto color{MONEY::GANG::Color::Svart};
//     if (color == MONEY::GANG::Color::Svart) {
//         printf("Svart som Svartalfheimr\n");
//     }
//
//     // Using directives
//     using MONEY::GANG::Color;
//     const auto farg{Color::Vit};
//     if (farg == Color::Vit) printf("Vit som isen av Jotunheimr\n");
//
//     // In global namespace
//     const Realm realm{"Vanaheim", Color::Gront};
//
//     if (isDarker(realm)) {
//         printf("%s is as dark as Svartalfheimr\n", realm.name);
//     } else {
//         printf("%s is not as dark as Svartalfheimr\n", realm.name);
//     }
//
//     return 0;
// }
/* Namespaces */

/* Type aliasing */

// using String = const char[260];
// using RealmColor = MONEY::GANG::Color;

// int main() {
//     const auto color{RealmColor::Gront};
//     String saying{"SHADOW WIZARD MONEY GANG"};
//
//     if (color == RealmColor::Gront) {
//         printf("%s\n", saying);
//     }
//
//     return 0;
// }

// Partial Template Application
template <typename To, typename From>
struct NarrowCaster {
    To cast(From value) const {
        const auto converted = static_cast<To>(value);
        const auto backwards = static_cast<From>(converted);
        if (value != backwards) throw std::runtime_error{"Narrowed!"};
        return converted;
    }
};

template <typename From>
using short_caster = NarrowCaster<short, From>;

// int main() {
//     try {
//         const short_caster<int> caster;
//         const auto cyclic_short = caster.cast(142857);
//         printf("cyclic_short: %d", cyclic_short);
//     } catch (std::runtime_error& e) {
//         printf("Exception: %s\n", e.what());
//     }
//     return 0;
// }

// Structed Bindings

struct TextFile {
    bool success;
    const char* contents;
    size_t n_bytes;
};

TextFile readTextFile(const char* path) {
    const static char contents[]{"Reinventing the wheel to run myself over"};
    return TextFile{true, contents, sizeof(contents)};
}

// int main() {
//     const auto [status, contents, length] = readTextFile("./Balla.txt");
//     if (status) {
//         printf("Bytes: %zu\nContents: %s\n", length, contents);
//
//     } else {
//         printf("Failed to open Balla.txt\n");
//     }
//     return 0;
// }

/* Type aliasing */

/* Attributes */

[[noreturn]] void pitcher() { throw std::runtime_error{"Knuckleball"}; }

int main() {
    try {
        pitcher();
    } catch (const std::exception& e) {
        printf("Exception: %s\n", e.what());
    }
    return 0;
}

/* Attributes */
