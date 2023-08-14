#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_set>

using namespace std;
using namespace std::filesystem;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: magic PATH";
        return -1;
    }

    const std::unordered_set<string> pe_extenstions{".acm", ".ax",  ".cpl", ".dll", ".drv", ".efi",
                                                    ".exe", ".mui", ".ocx", ".scr", ".sys", ".tsp"};

    const path sys_path{argv[1]};
    cout << "Searching " << sys_path << " recursivley ";
    size_t n_searched{};
    auto itr = recursive_directory_iterator{sys_path, directory_options::skip_permission_denied};
    for (const auto& entry : itr) {
        try {
            if (!entry.is_regular_file()) continue;
            const auto& extenstion = entry.path().extension().string();
            const auto is_pe = pe_extenstions.find(extenstion) != pe_extenstions.end();

            if (!is_pe) continue;
            ifstream file{entry.path()};
            char first{}, second{};

            if (file) file >> first;
            if (file) file >> second;

            if (first != 'M' || second != 'Z') cout << "Invalid PE found: " << entry.path().string() << "\n";
            n_searched++;

        } catch (const exception& e) {
            cerr << "Exception reading " << entry.path().string() << ": " << e.what() << endl;
        }
    }

    return 0;
}
