#include <deque>
#include <iostream>
#include <ranges>
#include <set>
#include <string>

#include "fundamental.hpp"
#include "timer.hpp"

using namespace std;
using namespace data_structures;
namespace rg = ranges;

bool areDelimsBalanced(const string& str) {
    const std::vector<char> open{'(', '{', '['};
    const std::vector<char> close{')', '}', ']'};
    base::LinkedStack<char> charStack{};

    for (const auto& c : str) {
        if (auto openItr = rg::find(open, c); openItr != open.end())
            charStack.push(c);
        else if (auto closeItr = rg::find(close, c); closeItr != close.end()) {
            if (charStack.isEmpty()) return false;

            auto openDelim = charStack.pop();
            auto i = distance(open.begin(), rg::find(open, openDelim));
            auto j = distance(close.begin(), closeItr);
            if (i != j) return false;
        }
    }

    return charStack.isEmpty();
}

template <typename T>
void remove(base::LinkedStack<T>& stack) {
    if (stack.isEmpty()) return;
    stack.pop();
    return remove(stack);
}

// int main() {
//     base::LinkedStack<int> stack{};
//     stack.push(10);
//     stack.push(20);
//     stack.push(30);
//     stack.push(40);
//     remove(stack);
//     cout << stack.size() << '\n';
//     return 0;
// }

int main() {
    data_structures::queue::LinkedPriorityQueue<int> pq{};
    pq.insert(10);
    pq.insert(30);
    pq.insert(150);
    pq.insert(2);
    return 0;
}

// int main() {
//     cout << areDelimsBalanced("({{{{}}})") << '\n';
//     cout << areDelimsBalanced("(}{)") << '\n';
//     return 0;
// }

// int main() {
//     data_structures::base::LinkedStack<string> stack{};
//     size_t size = 10'000'000;
//     for (size_t i{}; i < size; i++) {
//         stack.push("Ballin");
//     }
//
//     for (size_t i{}; i < size; i++) {
//         stack.pop();
//     }
//
//     return 0;
// }

// int main() {
//     data_structures::tree::LinkedBinaryTree<char> tree{'A'};
//     auto root = tree.root();
//     auto left = tree.addLeft(root, 'B');
//     auto right = tree.addRight(root, 'C');
//     auto l1 = tree.addLeft(left, 'D');
//     auto r1 = tree.addRight(left, 'E');
//     auto l2 = tree.addLeft(right, 'F');
//     auto r2 = tree.addRight(right, 'G');
//     auto l3 = tree.addLeft(l1, 'H');
//     auto r3 = tree.addRight(l1, 'I');
//     auto l4 = tree.addLeft(r1, 'J');
//     auto r4 = tree.addRight(r1, 'K');
//     auto l5 = tree.addLeft(l2, 'L');
//     auto r5 = tree.addRight(l2, 'M');
//     auto l6 = tree.addLeft(r2, 'N');
//     auto r6 = tree.addRight(r2, 'O');
//     auto l7 = tree.addLeft(l3, 'P');
//     auto r7 = tree.addRight(l3, 'Q');
//     auto l8 = tree.addLeft(r3, 'R');
//     auto r8 = tree.addRight(r3, 'S');
//     auto l9 = tree.addLeft(l4, 'T');
//     auto r9 = tree.addRight(l4, 'U');
//     auto l10 = tree.addLeft(r4, 'V');
//     auto r10 = tree.addRight(r4, 'W');
//     auto l11 = tree.addLeft(l5, 'X');
//     auto r11 = tree.addRight(l5, 'Y');
//     auto l12 = tree.addLeft(r5, 'Z');
//     cout << tree.preorderDisplay() << '\n';
//     cout << tree.size() << '\n';
//     for (const auto& i : tree.preorder()) cout << string{*(i)} << ", ";
//     cout << '\n';
//     for (const auto& i : tree.postorder()) cout << string{*(i)} << ", ";
//     cout << '\n';
//     for (const auto& i : tree.inorder()) {
//         cout << string{*(i)} << ", ";
//     }
//     cout << '\n';
//     cout << tree.depth(tree.root()) << '\n';
//     cout << format("Depth {}: {}\n", string{*l12}, tree.depth(l12));
//     cout << format("Height {}: {}\n", string{*l12}, tree.height(l12));
//
//     cout << format("Depth {}: {}\n", string{*tree.root()}, tree.depth(tree.root()));
//     cout << format("Height {}: {}\n", string{*tree.root()}, tree.height(tree.root()));
//     return 0;
// }

// int main() {
//     ArrayList<size_t> list{};
//     data_structures::list::LinkedList<size_t> ll{};
//     data_structures::base::list::SinglyLinkedList<size_t> sl{};
//     data_structures::base::list::DoublyLinkedList<size_t> dl{};
//
//     size_t size = 10'000'000;
//
//     auto func = [&size]<typename T>(data_structures::list::List<T>* list, const string& name) {
//         {
//             TimerClass timer{name};
//             for (size_t i{0}; i < size; i++) list->add(i + 1);
//         }
//
//         {
//             TimerClass timer{name};
//             for (size_t i{0}; i < size; i++) list->remove(list->size() - 1);
//         }
//     };
//
//     func(&dl, "DoublyLinkedList");
//     func(&list, "ArrayList");
//     func(&sl, "SinglyLinkedList");
//
//     for (size_t i{1}; i < 100; i++) {
//         list.add(i);
//         ll.add(i);
//         sl.add(i);
//         dl.add(i);
//     }
//
//     for (size_t i{1}; i < 1000; i++) sl.add(i);
//
//     auto res = list.remove(10);
//     res = ll.remove(40);
//     ll.remove(0);
//     ll.remove(ll.size() - 1);
//     list.at(50) = 3210;
//     ll.at(23) = 2393;
//     sl.insert(39, 3943843);
//     sl.at(39) = 342349823842;
//     dl.insert(0, 19323233);
//     dl.insert(dl.size(), 29323834);
//     dl.insert(39, 132874);
//     dl.remove(dl.size() - 1);
//     dl.remove(0);
//     sl.remove(sl.size() - 1);
//     cout << list.at(50) << '\n';
//     cout << list.size() << '\n';
//     cout << ll.at(39) << '\n';
//     cout << ll.size() << '\n';
//     cout << sl.size() << '\n';
//     cout << dl.size() << '\n';
//     cout << dl.at(dl.size() - 1) << '\n';
//     cout << string{dl} << '\n';
//     cout << string{sl} << '\n';
//     cout << string{ll} << '\n';
//     cout << string{list} << '\n';
//     return 0;
// }
