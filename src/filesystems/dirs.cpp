// 
// #include <catch2/catch_all.hpp>
#include <chrono>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
using std::cout, std::endl, std::boolalpha, std::cerr;

/* Compositon */

// int main() {
//     try {
//         auto temp_path = fs::temp_directory_path();
//         const auto relative = fs::relative(temp_path);
//         cout << boolalpha << "\nTemp Directory Path: " << temp_path
//              << "\nTemp Directory absolute: " << temp_path.is_absolute() << "\nCurrent path: " << fs::current_path()
//              << "\nTemp Directory's relative path: " << temp_path.relative_path() << "\nRelative dir: " << relative
//              << "\nRelative dir is relative" << relative.is_relative() << "\nChanging current dir to temp";
//
//         fs::current_path(temp_path);
//         cout << "\nCurrent Directory: " << fs::current_path() << endl;
//
//     } catch (const std::exception& e) {
//         cerr << "Exception: " << e.what() << endl;
//         return 1;
//     }
//     return 0;
// }

/* Compositon */

/* File types */

void describe(const fs::path& p) {
    cout << boolalpha << "\nPath: " << p << endl;
    try {
        cout << "Is Directory: " << fs::is_directory(p) << endl;
        cout << "Is regular file: " << fs::is_regular_file(p) << endl;
    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
}

// int main() {
//     fs::path win_path{R"(C:/Windows/System32/kernel32.dll)"};
//     describe(win_path);
//     win_path.remove_filename();
//     describe(win_path);
//
//     fs::path nix_path{R"(/bin/bash)"};
//     describe(nix_path);
//     nix_path.remove_filename();
//     describe(nix_path);
//
//     return 0;
// }

/* File types */

/* Files and Directories */

using namespace std::chrono;

void writeInfo(const fs::path& p) {
    if (!fs::exists(p)) {
        cout << p << " does not exist." << endl;
        return;
    }

    const auto lastWrite = fs::last_write_time(p).time_since_epoch();
    const auto inHours = duration_cast<hours>(lastWrite).count();
    cout << p << "\t" << inHours << "\t" << fs::file_size(p) << "\n";
}

// Manipulating
int main() {
    const fs::path sourcePath{R"(/home/mads/projects/C++/Learning/build/src/filesystems/dirs)"};
    const auto readmePath = fs::temp_directory_path() / "README";

    try {
        writeInfo(sourcePath);
        writeInfo(readmePath);

        cout << "Copying " << sourcePath.filename() << " to " << readmePath.filename() << "\n";
        fs::copy(sourcePath, readmePath);
        writeInfo(readmePath);

        cout << "Resizing " << readmePath.filename() << "\n";
        fs::resize_file(readmePath, 1024);
        writeInfo(readmePath);

        cout << "Removing " << readmePath.filename() << "\n";
        fs::remove(readmePath);
        writeInfo(readmePath);

    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}

/* Files and Directories */
