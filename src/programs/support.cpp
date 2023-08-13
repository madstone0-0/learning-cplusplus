#include <cstdlib>
#include <iostream>
#include <string>

using std::cout, std::endl, std::string;
struct Tracer {
    Tracer(std::string name_in) : name{std::move(name_in)} { std::cout << name << " constructed.\n"; }
    ~Tracer() { std::cout << name << " destructed.\n"; }

   private:
    const std::string name;
};

// Tracer static_tracer{"static Tracer"};

void run() {
    cout << "Registering callback\n";
    // Termination callback
    std::atexit([] { cout << "***std::atexit callback executing***" << endl; });
    cout << "Callback registered\n";
    // Exit not from main
    // std::exit(0);
    // Nuclear
    // std::abort();
}

// int main() {
//     cout << "Entering main()\n";
//     Tracer localTracer{"localTracer"};
//     thread_local Tracer threadLocalTracer{"threadLocalTracer"};
//     const auto* dynamicTracer = new Tracer{"dynamicTracer"};
//
//     delete dynamicTracer;
//     run();
//     cout << "Exiting main()" << endl;
// }

/* Communicating with the environment */

// std::system
// int main() {
//     string command{"ping -c 4 google.com"};
//     const auto res = std::system(command.c_str());
//     cout << "The command \"" << command << "\" returned " << res << endl;
//     return 0;
// }

// std::getenv
// int main() {
//     string env{"PATH"};
//     string res{std::getenv(env.c_str()) != nullptr ? std::getenv(env.c_str()) : " nothing "};
//     cout << "The var " << env << " equals " << res << endl;
// }

/* Signals */

#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>

std::atomic_bool interrupted{};

extern "C" void handler(int signal) {
    cout << "Handler invoked with signal " << signal << endl;
    interrupted = true;
}

int main() {
    using namespace std::chrono_literals;

    std::signal(SIGINT, handler);
    std::signal(SIGKILL, handler);
    std::signal(SIGHUP, handler);
    std::signal(SIGBUS, handler);
    while (!interrupted) {
        cout << std::flush << "Waiting...\r\n";
        std::this_thread::sleep_for(1s);
    }
    cout << "Interrupted!\n";

    return 0;
}

/* Signals */

/* Communicating with the environment */
