#include <cstddef>
#include <format>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <utility>

using std::unique_ptr;

template <typename T>
class ResizebleArray {
   public:
    const static int INIT_SIZE = 10;

    ResizebleArray() : size{0}, MIN_SIZE{INIT_SIZE}, array{new T[INIT_SIZE]} {}

    explicit ResizebleArray(size_t size) : array{new T[size]}, size{0}, MIN_SIZE{size} {}

    // ~ResizebleArray() { array.reset(); }

    void add(const T& elem) {
        increaseIfFull();
        array[size] = elem;
        size++;
    }

    T remove(size_t index) {
        throwIfOutOfBounds(index);
        T removed = array[index];
        size_t i = index;
        while (i < size) {
            array[index] = array[index + 1];
            i++;
        }
        array[index] = nullptr;
        size--;
        return removed;
    }

    T& at(size_t index) {
        throwIfOutOfBounds(index);
        return &array[index];
    }

    operator std::string() {
        std::string str = "[";
        for (size_t i{}; i < size; i++) {
            str += std::format("{}, ", array[i]);
        }
        str += "]";
        return str;
    }

    [[nodiscard]] size_t length() const { return size; }

   private:
    void throwIfOutOfBounds(size_t index) {
        if (index >= size || size < 0) throw std::out_of_range(std::format("Index {} is out of range", index));
    }

    template <size_t n>
    size_t length(T (&)[n]) {
        return n;
    }

    void increaseIfFull() {
        if (size == MIN_SIZE) {
            MIN_SIZE *= 2;
            unique_ptr<T[]> newArray = std::make_unique<T[]>(MIN_SIZE);
            for (size_t i{}; i < size; i++) newArray[i] = array[i];
            array = std::move(newArray);
        }
    }
    size_t size;
    size_t MIN_SIZE;
    unique_ptr<T[]> array;
};

int main() {
    ResizebleArray<size_t> list{};
    for (size_t i{}; i < 5'000'000; i++) list.add(i);
    // std::cout << std::string(list) << '\n';
    return 0;
}
