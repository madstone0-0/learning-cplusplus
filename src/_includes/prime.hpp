struct PrimeNumberIterator {
    bool operator!=(int) const;
    PrimeNumberIterator& operator++();
    int operator*() const;

   private:
    int current{2};
};

struct PrimeNumberRange {
    explicit PrimeNumberRange(int);
    PrimeNumberIterator begin() const;
    int end() const;

   private:
    const int max;
};
