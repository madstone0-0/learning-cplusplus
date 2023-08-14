#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <utility>

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

    // Move constructor
    SimpleString(SimpleString&& other) noexcept
        : max_size{other.max_size}, buffer(other.buffer), length(other.length) {
        other.length = 0;
        other.max_size = 0;
        other.buffer = nullptr;
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

    // Move assignment
    SimpleString& operator=(SimpleString&& other) noexcept {
        if (this == &other) return *this;
        delete[] buffer;
        buffer = other.buffer;
        length = other.length;
        max_size = other.max_size;
        other.buffer = nullptr;
        other.max_size = 0;
        other.length = 0;
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
    size_t length;
    char* buffer;
};

struct SimpleStringOwner {
    SimpleStringOwner(const char* x) : string{10} {
        if (!string.append_line(x)) {
            throw std::runtime_error("Not enough memory");
        }
        string.print("Constructed");
    }

    SimpleStringOwner(const SimpleString& my_string) : string{my_string} {}

    SimpleStringOwner(SimpleString&& x) : string{std::move(x)} {}

    ~SimpleStringOwner() { string.print("About to destroy"); }

   private:
    SimpleString string;
};

void ownAString() {
    SimpleString a{50};
    a.append_line("We apologize for the");
    a.append_line("inconvenience");
    SimpleStringOwner b{a};
}

// Lvalue parameters are denoted by a prepending &
void refType(int& x) { printf("lvalue reference %d\n", x); }

// Rvalue parameters are denoted by a prepending &&
void refType(int&& x) { printf("rvalue reference %d\n", x); }

int main() {
    ownAString();

    SimpleString a{50};
    // a is a lvalue (left value), i.e. a named value
    SimpleStringOwner b{a};
    // SimpleString{50} is a rvalue (right value)
    SimpleStringOwner c{SimpleString{50}};
    auto d{5};
    refType(9);
    refType(d);
    refType(d + 5);
    // Casting an lvalue to an rvalue
    refType(std::move(d));

    // Moving
    SimpleString e{50};
    e.append_line("SHADOW WIZARD");
    SimpleString f{50};
    f.append_line("WIZARD MONEY GANGG");
    e.print("e");
    f.print("f");
    f = std::move(e);
    f.print("f");

    SimpleString g{50};
    SimpleStringOwner h{std::move(g)};

    return 0;
}
