#include <deque>
#include <ios>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
ostream& operator<<(ostream& s, const vector<T>& v) {
    s << "Size: " << v.size() << "\nCapacity: " << v.capacity() << "\nElements: \n";
    for (const auto& element : v) s << "\t" << element << "\n";
    return s;
}

// int main() {
//     const vector<string> characters{"Bobby Shaftoe", "Lawrence Waterhouse", "Gunter Bischoff", "Earl Comstock"};
//     cout << characters << endl;
//
//     const vector<bool> bits{true, false, true, false};
//     cout << boolalpha << bits << endl;
// }

template <typename T>
istream& operator>>(istream& s, deque<T>& d) {
    T element;
    while (s >> element) d.emplace_back(std::move(element));
    return s;
}

int main() {
    cout << "Give me numbers: ";
    deque<int> numbers;
    cin >> numbers;
    int sum{};
    cout << "Cumulative sum:\n";
    for (const auto& element : numbers) {
        sum += element;
        cout << sum << "\n";
    }
}
