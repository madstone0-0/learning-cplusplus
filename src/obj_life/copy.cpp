#include <cstdio>
#include <cstring>
#include <stdexcept>

// Copy op
int addOneTo(int x) {
    x++;
    return x;
}

struct SimpleString {
    SimpleString(size_t max_size) : max_size{max_size}, length{} {
        if (max_size == 0) {
            throw std::runtime_error("Max size must be at least 1");
        }
        buffer = new char[max_size];
        buffer[0] = 0;
    }

    // Copy constructor
    SimpleString(const SimpleString& other)
        : max_size{other.max_size},
          buffer{new char[other.max_size]},
          length{other.length} {
        std::strncpy(buffer, other.buffer, max_size);
    }

    // Copy assignment
    SimpleString& operator=(const SimpleString& other) {
        if (this == &other) return *this;
        const auto new_buffer = new char[other.max_size];
        delete[] buffer;
        buffer = new_buffer;
        length = other.length;
        max_size = other.max_size;
        std::strncpy(buffer, other.buffer, max_size);

        return *this;
    }

    ~SimpleString() { delete[] buffer; }

    void print(const char* tag) const { printf("%s: %s\n", tag, buffer); }

    bool append_line(const char* x) {
        const auto x_len = strlen(x);
        if (x_len + length + 2 > max_size) return false;
        std::strncpy(buffer + length, x, max_size - length);
        length += x_len;
        buffer[length++] = '\n';
        buffer[length] = 0;
        return true;
    }

   private:
    size_t max_size;
    char* buffer;
    size_t length;
};

// Default Copy
struct Replicant {
    // Explicitly stated replicant can use the compiler assigned copy
    // constructor
    Replicant(const Replicant&) = default;
    Replicant& operator=(const Replicant&) = default;
};

struct Highlander {
    // The Highlander class should not be copied so we delete the default
    // copy constructor
    Highlander(const Highlander&) = delete;
    Highlander& operator=(const Highlander&) = delete;
};

int main() {
    auto og = 1;
    auto res = addOneTo(og);
    printf("Og: %d\nRes: %d\n", og, res);

    // Copy constructors
    SimpleString a{50};
    a.append_line("We apologize for the");
    SimpleString a_copy{a};
    a.append_line("inconvenience");
    a_copy.append_line("Balls");
    a.print("a");
    a_copy.print("a_copy");

    // Copy assignment
    SimpleString b{50};
    b.append_line("SHADOW");
    SimpleString c{b};
    c.append_line("WIZARD");
    b = c;
    b.print("b");
    c.print("c");
    return 0;
}
