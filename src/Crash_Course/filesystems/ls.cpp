#include <chrono>
#include <concepts>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <type_traits>

// #include "bytes.hpp"

namespace fs = std::filesystem;
using std::cout, std::endl, std::boolalpha, std::cerr, std::setw, std::string, std::map, std::string_view,
    std::stringstream;
using namespace std::chrono;

template <typename T>
concept PathLike = requires(T a, T b) {
    { fs::is_empty(a) } -> std::convertible_to<bool>;
    { fs::exists(a) } -> std::convertible_to<bool>;
    { fs::file_size(a) } -> std::convertible_to<int>;
};

template <PathLike Path>
void describe(const Path& entry, bool humanReadable = false) {
    try {
        if (fs::is_directory(entry)) {
            cout << "           *";
        } else {
            auto file_size_bytes = fs::file_size(entry);
            // auto [file_size_mb, file_size_unit] = Bytes::convert(file_size_bytes, "B", "MB");
            cout << setw(12) << file_size_bytes;
        }

        auto lwTime = duration_cast<seconds>(fs::last_write_time(entry).time_since_epoch());

        if (humanReadable) {
            stringstream humanTime;
            auto inTimeT = system_clock::to_time_t(system_clock::time_point{lwTime});
            humanTime << std::put_time(std::localtime(&inTimeT), "%r");

            cout << setw(12) << humanTime.str() << " " << entry.filename().string() << "\n";

            return;
        }

        cout << setw(12) << lwTime.count() << " " << entry.filename().string() << "\n";

    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
}

map<char, bool> parseArgs(string_view v) {
    static map<char, bool> args{{'R', false}, {'h', false}};
    stringstream ss;
    ss << v;
    char s{};
    while (ss >> s) {
        try {
            args.at(s) = !args.at(s);
        } catch (const std::out_of_range&) {
            continue;
        }
    }
    return args;
}

template <typename T>
concept DirectoryIterator = requires(T a) {
    { *a } -> std::convertible_to<fs::directory_entry>;
    a++;
};

template <typename T>
concept Predicate = requires(T a) {
    { a() } -> std::convertible_to<bool>;
};

template <DirectoryIterator Itr, Predicate Pred>
int listItems(Itr itr, string errMsg, Pred predicate) {
    if (predicate()) {
        for (const auto& entry : itr) describe(entry.path());
        return 0;
    } else {
        cerr << errMsg << endl;
        return -1;
    }
}

int main(int argc, char* argv[]) {
    if (!(argc <= 3 && argc >= 2)) {
        cerr << "Usage: ls PATH or ls -OPTIONS PATH\n";
        return -1;
    }

    try {
        const string arg1{argv[1]};

        cout << "Size         Last Write  Name\n"
             << "------------ ----------- ------------\n";

        if (argc == 3) {
            const fs::path sys_path{argv[2]};
            if (*(arg1.begin()) != '-') {
                cerr << "Usage: ls PATH or ls -OPTIONS PATH\n";
                return -1;
            }

            auto args = parseArgs({arg1.begin() + 1, arg1.end()});

            if (args.at('R')) {
                return listItems(fs::recursive_directory_iterator{sys_path}, "Cannot recursively list a file",
                                 [&sys_path]() { return fs::is_directory(sys_path); });
            }

            if (args.at('h')) {
                if (fs::is_directory(sys_path)) {
                    auto itr = fs::directory_iterator{sys_path};
                    for (const auto& entry : fs::directory_iterator{sys_path}) describe(entry.path(), true);
                } else if (fs::is_regular_file(sys_path))
                    describe(sys_path, true);
                return 0;
            }

            if (fs::is_directory(sys_path)) {
                auto itr = fs::directory_iterator{sys_path};
                for (const auto& entry : fs::directory_iterator{sys_path}) describe(entry.path());
            } else if (fs::is_regular_file(sys_path))
                describe(sys_path);
            return 0;
        }

        const fs::path sys_path{argv[1]};
        if (fs::is_directory(sys_path))
            for (const auto& entry : fs::directory_iterator{sys_path}) describe(entry.path());
        else if (fs::is_regular_file(sys_path))
            describe(sys_path);

    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
