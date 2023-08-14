#include <chrono>
#include <concepts>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <type_traits>

#include "bytes.hpp"

namespace fs = std::filesystem;
using std::cout, std::endl, std::boolalpha, std::cerr, std::setw;
using namespace std::chrono;

template <typename T>
concept PathLike = requires(T a, T b) {
    { fs::is_empty(a) } -> std::convertible_to<bool>;
    { fs::exists(a) } -> std::convertible_to<bool>;
    { fs::file_size(a) } -> std::convertible_to<int>;
};

template <PathLike Path>
void describe(const Path& entry) {
    try {
        if (fs::is_directory(entry)) {
            cout << "           *";
        } else {
            auto file_size_bytes = fs::file_size(entry);
            auto [file_size_mb, file_size_unit] = Bytes::convert(file_size_bytes, "B", "MB");
            cout << setw(12) << std::setprecision(2) << file_size_mb;
        }

        const auto lwTime = duration_cast<seconds>(fs::last_write_time(entry).time_since_epoch());
        cout << setw(12) << lwTime.count() << " " << entry.filename().string() << "\n";

    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: listdir PATH\n";
        return -1;
    }

    try {
        const fs::path sys_path{argv[1]};
        cout << "Size         Last Write  Name\n"
             << "------------ ----------- ------------\n";
        if (fs::is_directory(sys_path))
            for (const auto& entry : fs::directory_iterator{sys_path}) describe(entry.path());
        else if (fs::is_regular_file(sys_path))
            describe(sys_path);

    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
