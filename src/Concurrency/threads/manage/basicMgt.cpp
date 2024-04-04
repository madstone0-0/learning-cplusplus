#include <fmt/core.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <numeric>
#include <ranges>
#include <ratio>
#include <thread>
#include <timer.hpp>
#include <vector>

using std::cout, std::endl, std::thread;
using std::string;
using std::vector;
namespace rg = std::ranges;

/* Basic thread management */

class BackgroundTask {
   public:
    void doSomething() const { cout << "Something" << '\n'; }

    void doSomethingElse() const { cout << "Something else" << '\n'; }

    void operator()() const {
        doSomething();
        doSomethingElse();
    }
};

class SomethingWithState {
   public:
    SomethingWithState() = default;
    explicit SomethingWithState(int state) : state{state} {}
    void increase() { state++; }
    void increase(int state) { this->state += state; }

    void decrease() { state--; }

    void decrease(int state) { this->state -= state; }

    [[nodiscard]] int getState() const { return state; }

   private:
    int state{};
};

// RAII thread wrapper
/* V1 */
// class ThreadGuard {
//     thread& t;
//
//    public:
//     explicit ThreadGuard(thread& t_) : t{t_} {}
//     ~ThreadGuard() {
//         if (t.joinable()) t.join();
//     }
//
//     ThreadGuard(const ThreadGuard& other) = delete;
//     ThreadGuard& operator=(const ThreadGuard& other) = delete;
// };
/* V1 */

/* V2 */
class ThreadGuard {
    thread t;

   public:
    explicit ThreadGuard(thread t_) : t{std::move(t_)} {
        if (!t.joinable()) throw std::logic_error("No thread");
    }
    ~ThreadGuard() { t.join(); }

    ThreadGuard(ThreadGuard&& other) noexcept : t{std::move(other.t)} {}

    ThreadGuard& operator=(ThreadGuard&& other) noexcept {
        if (t.joinable()) t.join();
        t = std::move(other.t);
        return *this;
    }

    ThreadGuard(const ThreadGuard& other) = delete;
    ThreadGuard& operator=(const ThreadGuard& other) = delete;
};
/* V2 */

// Joining thread sample impl
class JoiningThread {
   private:
    thread t;

   public:
    JoiningThread() noexcept = default;
    JoiningThread(JoiningThread&) = delete;
    JoiningThread& operator=(JoiningThread&) = delete;
    explicit JoiningThread(thread t_) noexcept : t{std::move(t_)} {}

    template <typename Callable, typename... Args>
    explicit JoiningThread(Callable&& func, Args&&... args)
        : t{std::forward<Callable>(func), std::forward<Args>(args)...} {}

    JoiningThread(JoiningThread&& other) noexcept : t{std::move(other.t)} {}

    JoiningThread& operator=(JoiningThread&& other) noexcept {
        if (joinable()) join();
        t = std::move(other.t);
        return *this;
    }

    JoiningThread& operator=(thread other) noexcept {
        if (joinable()) join();
        t = std::move(other);
        return *this;
    }

    ~JoiningThread() noexcept {
        if (joinable()) join();
    }

    void swap(JoiningThread& other) noexcept { t.swap(other.t); }

    [[nodiscard]] thread::id get_id() const noexcept { return t.get_id(); }

    void detach() { t.detach(); }

    thread& as_thread() noexcept { return t; }

    [[nodiscard]] const thread& as_thread() const noexcept { return t; }

    [[nodiscard]] bool joinable() const { return t.joinable(); }

    void join() { return t.join(); }
};

unsigned long long factorial(unsigned long long n) {
    unsigned long long res{0};
    if (n < 1)
        return 1;
    else
        res = n * factorial(n - 1);
    return res;
}

// Parallel version of std::accumulate
template <typename Iterator, typename T>
struct AccumulateBlock {
    void operator()(Iterator beg, Iterator end, T& result) { result = std::accumulate(beg, end, result); }
};

template <typename Iterator, typename T>
T parallelAccumulate(Iterator beg, Iterator end, T init) {
    size_t len = std::distance(beg, end);

    if (!len) return init;

    const size_t minPerThread{25};
    const size_t maxThreads{(len + minPerThread - 1) / minPerThread};
    const size_t hardwareThreads{thread::hardware_concurrency()};
    const size_t numThreads{std::min(hardwareThreads == 0 ? 2 : hardwareThreads, maxThreads)};

    const size_t blockSize{len / numThreads};
    vector<T> results(numThreads);
    vector<thread> threads(numThreads - 1);
    Iterator blockStart{beg};

    for (size_t i{}; i < numThreads - 1; i++) {
        Iterator blockEnd{blockStart};
        std::advance(blockEnd, blockSize);
        threads.at(i) = thread(AccumulateBlock<Iterator, T>(), blockStart, blockEnd, std::ref(results.at(i)));
        blockStart = blockEnd;
    }

    AccumulateBlock<Iterator, T>()(blockStart, end, results[numThreads - 1]);
    for (auto& t : threads) t.join();

    return std::accumulate(results.begin(), results.end(), init);
}

int main() {
    // Thread works with any callable object
    BackgroundTask task;
    thread taskThread{task};
    auto l = [] { cout << "Lambda Something" << '\n'; };
    auto wrapped = [] { cout << "Wrapped Lambda" << '\n'; };
    auto paramL = [](std::string word) { cout << word << '\n'; };
    auto paramLRef = [](std::string& word) {
        word = "Ballin";
        cout << word << '\n';
    };

    std::string word = "Not Balling";
    thread lambdaThread{[&l]() { l(); }};
    thread wrappedThread{wrapped};

    // Passing arguments to threads
    thread paramThread{paramL, word};
    thread paramRefThread{paramLRef, std::ref(word)};

    // Invoking a member function using a suitable object pointer
    thread memberThread{&BackgroundTask::doSomethingElse, &task};

    // Moving arguments to threads
    std::unique_ptr<SomethingWithState> something{new SomethingWithState{}};
    auto processSomething = [](std::unique_ptr<SomethingWithState> something) {
        something->decrease(20);
        cout << "Something: " << something->getState() << '\n';
    };
    something->increase(10);
    thread somethingThread{processSomething, std::move(something)};

    // Transferring thread ownership
    thread t1{paramL, string{"t1"}};
    thread t2{std::move(t1)};
    t1 = thread{paramL, string{"t2"}};
    thread t3;
    t3 = std::move(t2);
    // t1 = std::move(t3); // Cannot move into a thread that is currently running something

    // Vector of threads
    auto factorialWorker = [](unsigned long long n) {
        auto res{factorial(n)};
        cout << "factorial(" << n << ") = " << res << '\n';
    };

    // std::vector<thread> threads;
    std::vector<JoiningThread> threads;
    for (size_t i{1}; i <= 25; i++) threads.emplace_back(factorialWorker, i);
    // for (auto& t : threads) t.join();

    try {
        ThreadGuard guared{std::move(wrappedThread)};
        ThreadGuard refGuard{std::move(paramRefThread)};

        ThreadGuard m{std::move(memberThread)};

        ThreadGuard s{std::move(somethingThread)};

        ThreadGuard g1{std::move(t1)};
        ThreadGuard g3{std::move(t3)};

        // throw STD::runtime_error("BOOM");
    } catch (const std::exception& e) {
        cout << "Abnormal execution" << '\n';
        // Waiting for a thread to exit in the event of an exception
        // on the current thread
        lambdaThread.join();
        taskThread.join();
        paramThread.join();
        std::cerr << "Exception: " << e.what() << endl;
        return 1;
    }

    lambdaThread.detach();
    taskThread.detach();
    paramThread.detach();
    cout << "Normal execution" << '\n';

    for (auto& t : threads) t.join();

    vector<int> numbers(10'000'000);
    std::iota(numbers.begin(), numbers.end(), 1);
    std::chrono::nanoseconds seqDur{};
    std::chrono::nanoseconds parDur{};
    unsigned long long res{};

    {
        TimerClass seq{"Sequential", &seqDur, true};
        res = std::accumulate(numbers.begin(), numbers.end(), 0L);
    }
    cout << "Seq: " << res << '\n';

    {
        TimerClass para{"Parallel", &parDur, true};
        res = parallelAccumulate(numbers.begin(), numbers.end(), 0L);
    }
    cout << "Para: " << res << '\n';
    cout << "Para - Seq = " << std::chrono::duration_cast<std::chrono::microseconds>(parDur - seqDur) << endl;
    return 0;
}

/* Basic thread management */
