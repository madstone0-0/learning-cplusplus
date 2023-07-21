#ifndef FIB_INCLUDE
#define FIB_INCLUDE

struct FibonacciIterator {
    bool operator!=(int) const;
    FibonacciIterator& operator++();
    int operator*() const;

   private:
    int current{0};
    int last{1};
};

struct FibonacciRange {
    explicit FibonacciRange(int);
    FibonacciIterator begin() const;
    int end() const;

   private:
    const int max;
};

#endif
