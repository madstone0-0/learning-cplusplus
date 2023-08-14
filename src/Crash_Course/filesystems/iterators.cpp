#include <filesystem>
#include <iostream>
#include <string_view>

using namespace std;
using namespace std::filesystem;

struct Attributes {
    Attributes& operator+=(const Attributes& other) {
        this->size_bytes += other.size_bytes;
        this->n_directories += other.n_directories;
        this->n_files += other.n_files;
        return *this;
    }

    size_t size_bytes;
    size_t n_directories;
    size_t n_files;
};

void print_line(const Attributes& attributes, string_view path) {
    cout << setw(14) << attributes.size_bytes << setw(7) << attributes.n_files << setw(7) << attributes.n_directories
         << " " << path << "\n";
}

Attributes explore(const directory_entry& directory) {
    Attributes attrs{};
    for (const auto& entry : recursive_directory_iterator{directory}) {
        if (entry.is_directory())
            attrs.n_directories++;
        else {
            attrs.n_files++;
            attrs.size_bytes += entry.file_size();
        }
    }
    return attrs;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: treedir PATH";
        return -1;
    }

    try {
        const path sys_path{argv[1]};

        cout << "Size         Last Write  Name\n"
             << "------------ ----------- ------------\n";
        Attributes root_attrs{};
        for (const auto& entry : directory_iterator{sys_path}) {
            if (entry.is_directory()) {
                const auto attrs = explore(entry);
                root_attrs += attrs;
                print_line(attrs, entry.path().string());
            } else {
                root_attrs.n_files++;
                error_code ec;
                root_attrs.size_bytes += entry.file_size(ec);
                if (ec) cerr << "Error reading file size: " << entry.path().string() << endl;
            }
        }

        print_line(root_attrs, argv[1]);

    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
