#include <cstdio>

struct ArgLength {
    ArgLength() : size{61}, lengths{new int[size]} {}
    ArgLength(size_t size) : size{size}, lengths{new int[size]} {}
    void ingest(const char* str);
    void print() const;
    ~ArgLength() { delete[] lengths; }

   private:
    const size_t size;
    int* lengths;
};

int main(int argc, char** argv) {
    ArgLength hist{40};
    for (size_t i{1}; i < argc; i++) hist.ingest(argv[i]);
    hist.print();

    return 0;
}

void ArgLength::ingest(const char* str) {
    size_t index{};
    while (str[index]) index++;
    lengths[index]++;
}

void ArgLength::print() const {
    for (auto i{1}; i < size; i++) {
        printf("%d\t", i);
        auto n_stars = lengths[i];
        while (n_stars--) printf("*");
        printf("\n");
    }
}
