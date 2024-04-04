#include <coroutine>
#include <iostream>

// https://en.cppreference.com/w/cpp/language/coroutines
template <typename T>
struct generator {
    struct promise_type {
        T value{};
        std::exception_ptr exec;
        generator get_return_object() { return generator(handle_type::from_promise(*this)); }

        std::suspend_always initial_suspend() { return {}; }

        std::suspend_always final_suspend() noexcept { return {}; }

        void unhandled_exception() { exec = std::current_exception(); }

        template <std::convertible_to<T> From>
        std::suspend_always yield_value(From&& from) {
            value = std::forward<From>(from);
            return {};
        }

        void return_void() {}
    };

    using handle_type = std::coroutine_handle<promise_type>;

    handle_type h;

    generator(handle_type h) : h{h} {}

    ~generator() { h.destroy(); }
    explicit operator bool() {
        fill();
        return !h.done();
    }

    T operator()() {
        fill();
        full = false;
        return std::move(h.promise().value);
    }

   private:
    bool full{false};

    void fill() {
        if (!full) {
            h();
            if (h.promise().exec) std::rethrow_exception(h.promise().exec);
            full = true;
        }
    }
};

generator<int> range(int start, int end) {
    while (start < end) {
        co_yield start;
        start++;
    }
}

int main() {
    using namespace std;
    try {
        auto gen = range(0, 10);
        for (auto i{0}; gen; i++) {
            cout << gen() << " ";
        }
        cout << '\n';
    } catch (const exception& e) {
        cerr << "Execption: " << e.what() << endl;
    }
}
