#include <condition_variable>
#include <future>
#include <initializer_list>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <vector>

using std::queue, std::async, std::launch, std::scoped_lock, std::mutex, std::move, std::cout, std::endl,
    std::condition_variable;

template <typename T>
struct ThreadQueue {
    ThreadQueue() = default;
    ThreadQueue(std::initializer_list<T> values) : data{values} {}

    ThreadQueue(const ThreadQueue& other) : data{other} {}

    ThreadQueue(ThreadQueue&& other) noexcept : data{move(other.data)} {}

    ThreadQueue& operator=(const ThreadQueue& other) {
        if (other == this) return *this;
        this->data = other.data;
        return *this;
    }

    ThreadQueue& operator=(ThreadQueue&& other) noexcept {
        this->data = move(other.data);
        return *this;
    }

    ~ThreadQueue() = default;

    void pop() {
        scoped_lock<mutex> lock{m};
        data.pop();
    }

    T wait_and_pop() {
        std::unique_lock<mutex> lock{m};
        cv.wait(lock, [this] { return !this->data.empty(); });
        T element = data.front();
        data.pop();
        return element;
    }

    void push(T&& x) {
        scoped_lock<mutex> lock{m};
        data.push(x);
        cv.notify_all();
    }

    T& front() {
        scoped_lock<mutex> lock{m};
        return data.front();
    }

    T& back() {
        scoped_lock<mutex> lock{m};
        return data.back();
    }

    size_t size() const { return data.size(); }

    bool empty() const { return data.empty(); }

   private:
    queue<T> data{};
    mutex m;
    condition_variable cv;
};

void do_shit() {
    ThreadQueue<int> q1{1, 2, 3, 4, 5, 6};
    ThreadQueue<int> q2{};

    auto add_shit = async(launch::async, [&] {
        q1.push(1);
        q1.push(3);
        q1.push(6);
        q1.push(2);
    });

    auto remove_shit = async(launch::async, [&] {
        q1.pop();
        q1.pop();
        q1.pop();
        q1.pop();
    });

    auto wait_then_remove = async(launch::async, [&] {
        std::stringstream ss;
        auto val = q2.wait_and_pop();
        ss << "Val: " << val << "\n";
        return ss.str();
    });

    auto stop_wait = async(launch::async, [&] {
        q2.push(8);
        q2.push(2);
        q2.push(5);
    });

    add_shit.get();
    remove_shit.get();
    auto res = wait_then_remove.get();
    stop_wait.get();

    cout << "Items in queue 1:\n( ";
    for (int i = static_cast<int>(q1.size() - 1); i >= 0; --i) {
        cout << q1.front() << " ";
        q1.pop();
    }
    cout << ")\n" << endl;

    cout << "Items in queue 2:\n( ";
    for (int i = static_cast<int>(q2.size() - 1); i >= 0; --i) {
        cout << q2.front() << " ";
        q2.pop();
    }
    cout << ")"
         << "\n"
         << res << endl;
}

int main() {
    do_shit();
    do_shit();
    do_shit();
    return 0;
}
