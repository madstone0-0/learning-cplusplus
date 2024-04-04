#include <algorithm>
#include <boost/type_index.hpp>
#include <deque>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

using std::cout;
namespace ranges = std::ranges;

void auth() { std::cout << "Authed" << std::endl; }

template <typename Container, typename Index>
decltype(auto) authAndAccess(Container&& con, Index i) {
    auth();

    return std::forward<Container>(con)[i];
}

struct Widget {
    size_t index{};
};

template <typename T>
void f(const T& param) {
    using boost::typeindex::type_id_with_cvr;
    using std::cout;
    // cout << "T =\t" << typeid(T).name() << '\n';
    // cout << "param =\t" << typeid(param).name() << '\n';

    cout << "T =\t" << type_id_with_cvr<T>() << '\n';
    cout << "param =\t" << type_id_with_cvr<decltype(param)>() << '\n';
}

std::vector<Widget> createWidget() {
    size_t index{};
    std::vector<Widget> widgets{};
    ranges::generate_n(std::back_inserter(widgets), 10, [&index] { return Widget{index++}; });
    return widgets;
}

int main() {
    using boost::typeindex::type_id_with_cvr;
    std::deque<int> intQueue{};
    for (int i{}; i < 10; i++) {
        intQueue.push_back(i);
    }
    auto val = authAndAccess(intQueue, 5);
    cout << "Val: " << val << std::endl;

    // Runtime
    // cout << "Type: " << typeid(val).name() << '\n';
    cout << "Type: " << type_id_with_cvr<decltype(val)>() << '\n';

    const auto vw = createWidget();

    if (!vw.empty()) f(&vw.at(0));

    return 0;
}
