#include <algorithm>
#include <boost/algorithm/searching/boyer_moore.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/program_options.hpp>
#include <condition_variable>
#include <exception>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <iterator>
#include <map>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace op = boost::program_options;
namespace fs = std::filesystem;
namespace ba = boost::algorithm;

using std::cout, std::endl, std::string, std::cerr, std::vector, std::regex, std::smatch, std::ifstream,
    std::stringstream, std::regex_search, std::string_view, std::map, std::future, std::launch, std::async;

template <typename Pattern>
using SearchFunctionType = map<fs::path, string> (*)(fs::path, Pattern, std::counting_semaphore<>&);

struct Result {
    fs::path item;
    bool status;
    string line;
};

template <typename T>
bool isReady(const future<T>& f) {
    if (f.valid()) {
        return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    } else {
        return false;
    }
}

class Semaphore {
   public:
    explicit Semaphore(size_t count) : count{count} {}
    void lock() {
        std::unique_lock<std::mutex> lock{mutex};
        cv.wait(lock, [this] { return count != 0; });
        count--;
    }

    void unlock() {
        std::unique_lock<std::mutex> lock{mutex};
        count++;
        cv.notify_one();
    }

   private:
    size_t count;
    std::condition_variable cv;
    std::mutex mutex;
};

ifstream open(const fs::path path, std::ios_base::openmode mode = std::ios_base::in) {
    ifstream file{path, mode};
    if (!file.is_open()) {
        string err{"Unable to open file "};
        err.append(path);
        throw std::runtime_error(err);
    }
    file.exceptions(std::ios::badbit);
    return file;
}

string readFile(ifstream& file) {
    stringstream contentSS{stringstream{} << file.rdbuf()};
    return contentSS.str();
}

vector<string> getLines(string corpus) {
    vector<string> lines;
    ba::split(lines, corpus, [](auto& c) { return c == '\n'; });
    return lines;
}

Result searchFile(fs::path item, string pattern) {
    if (!fs::is_regular_file(item)) return {fs::path{item}, false, ""};

    boost::algorithm::boyer_moore bm{pattern.begin(), pattern.end()};

    auto contentStream = open(item);
    auto contentString = readFile(contentStream);
    auto lines = getLines(contentString);

    for (const auto& line : lines) {
        const auto [resBeg, resEnd] = bm(line);
        if (resBeg != resEnd) {
            if (line.size() >= 120) {
                auto res = Result{fs::path{item}, true, string{line.begin(), line.begin() + 120}};
                return res;
            }
            auto res = Result{fs::path{item}, true, line};
            return res;
        }
    }

    return {fs::path{""}, false, ""};
}

Result searchFile(fs::path item, regex pattern) {
    if (!fs::is_regular_file(item)) return {fs::path{item}, false, ""};
    smatch regexResults;

    auto contentStream = open(item);
    string contentString = readFile(contentStream);
    std::replace(contentString.begin(), contentString.end(), '\n', ' ');
    const auto result = regex_search(contentString, regexResults, pattern);
    return {fs::path{item}, result, string{regexResults.str()}};
}

template <typename Pattern>
map<fs::path, string> searchDir(fs::path dir, Pattern pattern, Semaphore& maxThreads) {
    map<fs::path, string> paths;
    vector<future<Result>> futures;
    vector<Result> readyFutures;

    for (const auto& item : fs::directory_iterator{dir}) {
        futures.push_back(async(
            launch::async,
            [](decltype(maxThreads)& maxThreads, auto item, auto pattern) {
                std::scoped_lock m(maxThreads);
                return searchFile(item, pattern);
            },
            std::ref(maxThreads), item, pattern));
    }

    do {
        for (auto& future : futures) {
            if (isReady(future)) readyFutures.push_back(future.get());
        }
    } while (readyFutures.size() < futures.size());

    for (const auto& result : readyFutures) {
        if (result.status) paths[result.item] = result.line;
    }

    return paths;
}

template <typename Pattern>
map<fs::path, string> searchDirRecursively(fs::path dir, Pattern pattern, Semaphore& maxThreads) {
    map<fs::path, string> paths;
    vector<future<Result>> futures;
    vector<Result> readyFutures;

    for (const auto& item : fs::recursive_directory_iterator{dir}) {
        futures.push_back(async(
            launch::async,
            [](decltype(maxThreads)& maxThreads, auto item, auto pattern) {
                std::scoped_lock m(maxThreads);
                return searchFile(item, pattern);
            },
            std::ref(maxThreads), item, pattern));
    }

    do {
        for (auto& future : futures) {
            if (isReady(future)) readyFutures.push_back(future.get());
        }
    } while (readyFutures.size() < futures.size());

    for (const auto& result : readyFutures) {
        if (result.status) paths[result.item] = result.line;
    }

    return paths;
}

template <typename Pattern>
auto searchWorker(fs::path p, Pattern pattern, Semaphore& maxThreads) {
    return async(
        launch::async,
        [](fs::path p, decltype(maxThreads)& maxThreads, auto pattern) {
            std::scoped_lock m(maxThreads);
            return searchDir(p, pattern, maxThreads);
        },
        std::move(p), std::ref(maxThreads), pattern);
}

template <typename Pattern>
auto searchRecWorker(fs::path p, Pattern pattern, Semaphore& maxThreads) {
    return async(
        launch::async,
        [](fs::path p, decltype(maxThreads)& maxThreads, auto pattern) {
            std::scoped_lock m(maxThreads);
            return searchDirRecursively(p, pattern, maxThreads);
        },
        std::move(p), std::ref(maxThreads), pattern);
}

int main(int argc, char** argv) {
    using namespace op;
    bool isRecursive{}, isHelp{}, isRegex{};

    options_description desc{"mgrep [options] pattern path1 path2..."};
    desc.add_options()("help,h", bool_switch(&isHelp), "display help")("threads,t", value<int>()->default_value(4),
                                                                       "number of threads to use")(
        "recursive,r", bool_switch(&isRecursive), "search subddirectories recursively")(
        "regex", bool_switch(&isRegex), "parse pattern as regex")("pattern", value<string>(), "search pattern")(
        "path", value<std::vector<string>>(), "paths to search");
    positional_options_description positional;
    positional.add("pattern", 1);
    positional.add("path", -1);

    command_line_parser parser{argc, argv};
    parser.options(desc);
    parser.positional(positional);

    variables_map vm;
    try {
        auto parsedResult = parser.run();
        store(parsedResult, vm);
        notify(vm);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << endl;
        return -1;
    }

    if (isHelp) {
        cout << desc;
        return 0;
    };

    if (vm["pattern"].empty()) {
        std::cerr << "You must provide a pattern" << endl;
        return -1;
    }

    if (vm["path"].empty()) {
        std::cerr << "You must provide at least one path" << endl;
        return -1;
    }

    const auto threads = vm["threads"].as<int>();
    auto& pattern = vm["pattern"].as<string>();
    const auto& path = vm["path"].as<std::vector<string>>();
    Semaphore maxThreads{static_cast<size_t>(threads)};

    if (threads < 2) {
        std::cerr << "Threads used must be at least 2 due to a bug" << endl;
        return -1;
    }

    regex re;
    try {
        re = regex{pattern};
    } catch (const std::regex_error&) {
        if (isRegex) {
            cerr << "Invalid regex" << endl;
            return -1;
        }
    }

    map<fs::path, string> paths;
    vector<future<decltype(paths)>> futures;
    vector<decltype(paths)> readyFutures;

    for (const auto& p : path) {
        if (!isRegex) {
            if (fs::is_directory(p) && !isRecursive) {
                futures.push_back(searchWorker(fs::path{p}, pattern, maxThreads));
            } else if (fs::is_directory(p) && isRecursive) {
                futures.push_back(searchRecWorker(fs::path{p}, pattern, maxThreads));
            } else if (fs::is_regular_file(p)) {
                auto searchResults = searchFile(p, pattern);
                if (!searchResults.status) continue;
                paths.insert({p, searchResults.line});
            } else {
                cerr << p << " cannot be searched." << endl;
                return -1;
            }
        } else {
            if (fs::is_directory(p) && !isRecursive) {
                futures.push_back(searchWorker(fs::path{p}, re, maxThreads));
            } else if (fs::is_directory(p) && isRecursive) {
                futures.push_back(searchRecWorker(fs::path{p}, re, maxThreads));
            } else if (fs::is_regular_file(p)) {
                auto searchResults = searchFile(p, pattern);
                if (!searchResults.status) continue;
                paths.insert({p, searchResults.line});
            } else {
                cerr << p << " cannot be searched." << endl;
                return -1;
            }
        }
    }

    try {
        do {
            for (auto& future : futures) {
                if (isReady(future)) readyFutures.push_back(future.get());
            }
        } while (readyFutures.size() < futures.size());

        for (const auto& results : readyFutures) {
            paths.insert(results.begin(), results.end());
        }
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }

    if (paths.size() > 1) {
        for (const auto& [path, line] : paths) {
            const auto displayPath = path.string().size() >= 120 ? path.filename().string() : path.string();
            cout << displayPath << ":" << line << "\n";
        }
    } else if (paths.size() == 1) {
        auto [path, line] = *(paths.begin());
        const auto displayPath = path.string().size() >= 120 ? path.filename().string() : path.string();

        cout << displayPath << ":" << line << "\n";
    }

    return 0;
}
