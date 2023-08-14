#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;
using std::cout, std::cerr, std::endl, std::string;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: search PATH EXT\n";
        return -1;
    }

    try {
        const fs::path path{argv[1]};
        const string ext{argv[2]};

        for (const auto& entry : fs::recursive_directory_iterator{path}) {
            try {
                if (!entry.is_regular_file()) continue;

                if (entry.path().extension() == ext) {
                    cout << entry.path().string() << endl;
                }

            } catch (const fs::filesystem_error& e) {
                continue;
            }
        }

    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
