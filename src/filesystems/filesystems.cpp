#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
using std::cout, std::endl;
/*std::filesystem*/

// Construction
TEST_CASE("std::filesystem::path supports == and empty()") {
    fs::path emptyPath{};
    fs::path shadowPath{"/etc/shadow"};
    REQUIRE(emptyPath.empty());
    REQUIRE(shadowPath == std::string{"/etc/shadow"});
}

// Decomposition
TEST_CASE("std::filesystem::path decompositon methods") {
    // const fs::path path{"/home/mads/aliases.linux.zsh"};
    const fs::path path{R"(C:\Windows\System32\kernel32.dll)"};
    cout << "Root name: " << path.root_name() << "\nRoot path: " << path.root_path()
         << "\nRelative path: " << path.relative_path() << "\nParent path: " << path.parent_path()
         << "\nFilename: " << path.filename() << "\nStem: " << path.stem() << "\nExtension: " << path.extension()
         << "\n"
         << endl;
}

// Modification
TEST_CASE("std::filesystem::path modification methods") {
    fs::path path{R"(C:\Windows\System32\kernel32.dll)"};
    cout << path << endl;

    path.make_preferred();
    cout << path << endl;

    path.remove_filename();
    cout << path << endl;

    path.replace_filename("win32kfull.sys");
    cout << path << endl;

    path.clear();
    cout << "Is empty: " << std::boolalpha << path.empty() << endl;
}

/*std::filesystem*/
