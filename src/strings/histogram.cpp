#include <cstdint>
#include <cstdio>
#include <map>
#include <string>
#include <string_view>

constexpr char pos_A{65}, pos_Z{90}, pos_a{97}, pos_z{122};
constexpr bool within_AZ(char x) { return pos_A <= x && pos_Z >= x; }

constexpr bool within_az(char x) { return pos_a <= x && pos_z >= x; }

using std::string, std::string_view;

struct AlphaHistogram {
    void ingest(string_view x) {
        for (const auto& c : x)
            if (within_az(c))
                counts[c - 32]++;
            else if (within_AZ(c))
                counts[c]++;
    };

    void print() const {
        for (auto index{pos_A}; index <= pos_Z; index++) {
            printf("%c\t", index);
            auto n_stars = counts.contains(index) ? counts.at(static_cast<char>(index)) : 0;
            while (n_stars > 0) {
                printf("*");
                n_stars--;
            }
            printf("\n");
        }
    };

   private:
    std::map<char, int> counts{};
};

int main(int argc, char** argv) {
    AlphaHistogram hist{};

    for (size_t i{1}; i < argc; i++) {
        hist.ingest(argv[i]);
    }
    hist.print();

    return 0;
}
