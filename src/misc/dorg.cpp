#include <chrono>
#include <filesystem>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <thread>
#include <vector>

namespace fs = std::filesystem;
using namespace std::chrono_literals;
using std::set, std::cerr, std::cout, std::map, std::string, std::endl;

// static const fs::path WORKING_DIR{"/home/mads/projects/test_bench"};
static const fs::path WORKING_DIR{"/home/mads/Downloads"};
static const map<string, set<string>> locations{
    {"Compressed", {"zip", "rar", "7zip", "iso", "jar", "7z", "gz", "xz", "bz2"}},
    {"Documents",
     {
         "pdf",
         "txt",
         "mscz",
         "crplugin",
         "xlsx",
         "qlplugin",
         "reg",
         "html",
         "docx",
         "xslx",
         "csv",
         "xml",
         "pptx",

     }},
    {"Music", {"mp3", "flac", "m4a", "ogg", "wav"}},
    {"Programs", {"exe", "msi", "img", "appinstaller", "appimage", "deb", "apk"}},
    {"Pictures", {"png", "jpg", "gif", "svg", "ico", "webp"}},
    {"Misc",
     {
         "bat", "js",      "cmd", "py",  "pext", "pthm", "lic", "rmskin",     "vbox-extpack", "mht",
         "xdx", "package", "pak", "cpp", "ttf",  "json", "gpg", "flatpakref", "sh",
     }},
    {"Video", {"mp4", "avi", "mkv"}},
    {"TBDTorrents", {"torrent"}}};

void createFolders(const fs::path& basePath) {
    for (const auto& [folder, exts] : locations) {
        try {
            fs::create_directory(basePath / folder);

        } catch (const fs::filesystem_error& e) {
            continue;
        } catch (const std::exception& e) {
            throw;
        }
    }
}

void moveToFolder(const fs::directory_entry& file) {
    for (const auto& [folder, exts] : locations) {
        string fileExt = file.path().extension().string();
        fileExt = string{fileExt.begin() + 1, fileExt.end()};

        if (exts.find(fileExt) != exts.end()) {
            try {
                cout << "Moving " << file.path().string() << " to " << folder << endl;
                const auto destFolder = fs::current_path() / fs::path{folder};
                const auto destName = file.path();
                const auto fullLoc = destFolder / destName.filename();
                fs::rename(file, fullLoc);

            } catch (const std::exception&) {
                throw;
            }
        }
    }
}

int main() {
    try {
        fs::current_path(WORKING_DIR);
        createFolders(fs::current_path());

        for (const auto& file : fs::directory_iterator{fs::current_path()}) {
            std::this_thread::sleep_for(0.01s);
            if (file.is_regular_file()) moveToFolder(file);
        }

    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
