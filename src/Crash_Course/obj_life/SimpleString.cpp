#include <cstdio>
#include <cstring>
#include <stdexcept>

struct SimpleString {
    SimpleString(size_t max_size) : max_size{max_size}, length{} {
        if (max_size == 0) {
            throw std::runtime_error("Max size must be at least 1");
        }
        buffer = new char[max_size];
        buffer[0] = 0;
    }

    ~SimpleString() { delete buffer; }

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

struct SimpleStringOwner {
    SimpleStringOwner(const char* x) : string{10} {
        if (!string.append_line(x)) {
            throw std::runtime_error("Not enough memory");
        }
        string.print("Constructed");
    }

    ~SimpleStringOwner() { string.print("About to destroy"); }

   private:
    SimpleString string;
};

void fn_c() { SimpleStringOwner c{"cccccccccccccc"}; }

void fn_b() {
    SimpleStringOwner b{"b"};
    fn_c();
}

int main() {
    SimpleString string{100};
    string.append_line("baller");
    string.print("1");
    string.append_line("BALLER");
    string.print("2");
    string.append_line("Legalize Nuclear Bombs");
    string.append_line("SHADOW WIZARD MONEY GANG");
    string.print("3");

    SimpleStringOwner x{"x"};
    printf("x is balling\n");

    // Call stack unwinding
    try {
        SimpleStringOwner a{"a"};
        fn_b();
        SimpleStringOwner d{"e"};
    } catch (const std::exception& e) {
        printf("Exception %s\n", e.what());
    }

    return 0;
}
