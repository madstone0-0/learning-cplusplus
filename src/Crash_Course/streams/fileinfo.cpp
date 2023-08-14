#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <exception>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using std::cerr, std::cout, std::string, std::ifstream, std::ios_base, std::endl, std::string_view, std::stringstream,
    boost::algorithm::split;

constexpr char pos_A{65}, pos_Z{90}, pos_a{97}, pos_z{122};
constexpr bool within_AZ(char x) { return pos_A <= x && pos_Z >= x; }

constexpr bool within_az(char x) { return pos_a <= x && pos_z >= x; }

ifstream open(const char *path, ios_base::openmode mode = ios_base::in) {
    ifstream file{path, mode};
    if (!file.is_open()) {
        string err{"Cannot open file "};
        err.append(path);
        throw std::runtime_error(err);
    }
    return file;
}

struct AlphaHistogram {
    void ingest(string_view x) {
        for (const auto &c : x)
            if (within_az(c))
                counts[c - 32]++;
            else if (within_AZ(c))
                counts[c]++;
    };

    void print() const {
        for (auto index{pos_A}; index <= pos_Z; index++) {
            printf("\t%c\t", index);
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

struct FileInfo {
    float avg_word_length;
    int word_count;
};

FileInfo getFileInfo(string_view s) {
    stringstream ss;
    ss << s;
    string str;

    int count{};
    float total{};
    while (ss >> str) {
        total += str.size();
        count++;
    }
    auto avg = total / count;

    return {avg, count};
}

int main(int argc, char *argv[]) {
    if (argc == 1 || argc > 3) {
        cerr << "Invalid number of arguments\n";
        return 1;
    }

    try {
        stringstream ss;
        AlphaHistogram hist;

        const string path{argv[1]};
        auto file = open(path.c_str());

        ss << file.rdbuf();
        auto contents = ss.str();

        hist.ingest(contents);
        auto [avg_word_length, word_count] = getFileInfo(contents);

        std::vector<string> tokens;
        auto is_forward_slash = [](auto c) { return c == '/'; };
        split(tokens, path, is_forward_slash);

        cout << "File name: " << tokens.back() << "\n";
        cout << "\t"
             << "Average Word Length: " << std::setprecision(2) << avg_word_length << "\n";
        cout << "\t"
             << "Word count: " << word_count << endl;
        hist.print();

    } catch (const std::exception &e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
