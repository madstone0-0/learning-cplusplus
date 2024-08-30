#include <iostream>
#include <string>

using std::cout, std::endl, std::string;

// Copy Swap Idiom
class SimpleString {
   public:
    SimpleString(size_t size = 0) : size{size}, cArray{size ? new char[size]{} : nullptr} {}

    SimpleString(const SimpleString& other) : size{other.size}, cArray{new char[other.size]} {
        std::copy(other.cArray, other.cArray + size, cArray);
   }

    SimpleString& operator=(SimpleString other) {
        swap(other, *this);
        return *this;
    }

    ~SimpleString() { delete[] cArray; }

    char& operator[](const size_t i) { return cArray[i]; }

   private:
    friend void swap(SimpleString& first, SimpleString& second) {
        using std::swap;

        swap(first.size, second.size);
        swap(first.cArray, second.cArray);
    }
    size_t size;
    char* cArray;
};

int main() {
    auto str{SimpleString{10}};
    auto copy{SimpleString{12}};
    copy = str;
    copy[0] = 'a';
    return 0;
}
