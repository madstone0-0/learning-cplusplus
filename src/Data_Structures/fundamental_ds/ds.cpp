#include <deque>
#include <iostream>
#include <random>
#include <ranges>
#include <set>
#include <string>

#include "structures/structures.hpp"
#include "timer.hpp"

using namespace std;
using namespace data_structures;
namespace rg = ranges;

struct Packet {
    int sequenceNum{};
    int data{};

    bool operator<=(const Packet& other) const { return sequenceNum <= other.sequenceNum; }
    bool operator>=(const Packet& other) const { return sequenceNum >= other.sequenceNum; }
    bool operator<(const Packet& other) const { return sequenceNum < other.sequenceNum; }
    bool operator>(const Packet& other) const { return sequenceNum > other.sequenceNum; }
};

int main() {
    data_structures::graph::directed::EdgeListDGraph<int, int> G{};
    auto i0 = G.addNode(0);
    auto i1 = G.addNode(1);
    auto i2 = G.addNode(2);
    auto i3 = G.addNode(3);

    auto o1 = G.addNode(4);
    auto o2 = G.addNode(5);

    auto h1 = G.addNode(6);
    auto h2 = G.addNode(7);
    auto h3 = G.addNode(8);
    auto h4 = G.addNode(9);

    auto e1 = G.addEdge(i0, h1, 0);
    auto e2 = G.addEdge(i0, h3, 0);

    auto e3 = G.addEdge(i1, h3, 0);

    auto e4 = G.addEdge(i2, h4, 0);

    auto e5 = G.addEdge(i3, h3, 0);
    auto e6 = G.addEdge(i3, h4, 0);

    auto e11 = G.addEdge(h3, h1, 0);
    auto e12 = G.addEdge(h3, h2, 0);
    auto e13 = G.addEdge(h3, h4, 0);

    auto e9 = G.addEdge(h1, o1, 0);

    auto e10 = G.addEdge(h4, o2, 0);
    auto e14 = G.addEdge(h4, o1, 0);

    auto e7 = G.addEdge(h2, o1, 0);
    auto e8 = G.addEdge(h2, o2, 0);

    auto input0 = G.validateNode(i0);
    cout << format("Indegree of Input 0: {}\nOutdegree of Input 0: {}\n", input0->inDegree(), input0->outDegree());

    auto sorted = G.toposort();

    for (const auto& i : sorted) {
        cout << **i << " ";
    }
    cout << '\n';

    int count{};
    G.dfs([&count](graph::NodePtr<int>& node) {
        // count++;
        // **node = count;
        cout << **node << '\n';
    });
    // G.dfs(
    //     [&count](graph::NodePtr<int>& node) {
    //         count++;
    //         **node = count;
    //     },
    //     F);

    // int i{};
    // for (const auto& n : G.nodes()) {
    //     cout << i << ": " << **n << '\n';
    //     i++;
    // }

    return 0;
}

// int main() {
//     heap::ArrayMinHeap<Packet> minHeap;
//     random_device rd;
//     mt19937 gen(rd());
//     uniform_int_distribution<int> dist(0, 100);
//     array<int, 20> arr{};
//     for (int i{}; i < 20; i++) {
//         arr.at(i) = dist(gen);
//         Packet packet{};
//         packet.sequenceNum = dist(gen);
//         packet.data = dist(gen);
//         minHeap.insert(packet);
//     }
//     auto size = minHeap.size();
//     for (size_t i{}; i < size; i++) {
//         cout << minHeap.extractExtreme().sequenceNum << " ";
//     }
//     cout << '\n';
//     return 0;
// }

// int main() {
//     tree::LinkedBinarySearchTree<int> bst{1};
//     srand(time(nullptr));
//     for (int i{}; i < 20; i++) {
//         bst.insert(rand() % 10);
//     }
//
//     for (const auto& i : bst.inorder()) cout << i->data << ", ";
//     cout << '\n';
//     cout << bst.preorderDisplay() << '\n';
//
//     auto res = bst.search(2);
//     if (res)
//         cout << res->data << '\n';
//     else
//         cout << "Not found\n";
//     bst.remove(2);
//     cout << bst.preorderDisplay() << '\n';
//     return 0;
// }

// int main() {
//     {
//         graph::directed::EdgeListDGraph<int, int> G{};
//         auto A = G.addNode(0);
//         auto B = G.addNode(0);
//         auto C = G.addNode(0);
//         auto D = G.addNode(0);
//         auto E = G.addNode(0);
//         auto F = G.addNode(0);
//
//         auto e1 = G.addEdge(A, B, 0);
//         auto e2 = G.addEdge(B, C, 0);
//         auto e3 = G.addEdge(D, C, 0);
//         auto e4 = G.addEdge(C, E, 0);
//         auto e5 = G.addEdge(F, B, 0);
//         auto e6 = G.addEdge(F, D, 0);
//
//         cout << "C should not be adjacent to D: " << C->isAdjacentTo(D) << '\n';
//         cout << "D should be adjacent to C: " << D->isAdjacentTo(C) << '\n';
//
//         int count{};
//         G.dfsStack([&count](graph::NodePtr<int>& node) {
//             count++;
//             **node = count;
//         });
//         // G.dfs(
//         //     [&count](graph::NodePtr<int>& node) {
//         //         count++;
//         //         **node = count;
//         //     },
//         //     F);
//
//         int i{};
//         for (const auto& n : G.nodes()) {
//             cout << char('A' + i) << ": " << **n << '\n';
//             i++;
//         }
//     }
//     {
//         graph::directed::EdgeListDGraph<int, int> G{};
//         auto A = G.addNode(0);
//         auto B = G.addNode(0);
//         auto C = G.addNode(0);
//         auto D = G.addNode(0);
//         auto E = G.addNode(0);
//         auto F = G.addNode(0);
//
//         auto e1 = G.addEdge(A, B, 0);
//         auto e2 = G.addEdge(B, C, 0);
//         auto e3 = G.addEdge(D, C, 0);
//         auto e4 = G.addEdge(C, E, 0);
//         auto e5 = G.addEdge(F, B, 0);
//         auto e6 = G.addEdge(F, D, 0);
//
//         int count{};
//         G.bfs([&count](graph::NodePtr<int>& node) {
//             count++;
//             **node = count;
//         });
//
//         int i{};
//         for (const auto& n : G.nodes()) {
//             cout << char('A' + i) << ": " << **n << '\n';
//             i++;
//         }
//     }
//     return 0;
// }

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

// int main() {
//     data_structures::queue::LinkedPriorityQueue<int> pq{};
//     pq.insert(10);
//     pq.insert(30);
//     pq.insert(150);
//     pq.insert(2);
//     return 0;
// }

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
