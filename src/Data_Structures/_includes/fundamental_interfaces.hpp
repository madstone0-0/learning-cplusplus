#include <cstddef>
#include <format>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <string>
#include <vector>

using std::shared_ptr, std::string, std::unique_ptr, std::function;

namespace data_structures {
    struct Sized {
       public:
        [[nodiscard]] virtual size_t size() const = 0;
        [[nodiscard]] virtual bool isEmpty() const = 0;
        void throwIfEmpty(const string& object, const string& verb) const {
            if (isEmpty()) throw std::runtime_error(std::format("Cannot {} from an empty {}", verb, object));
        }

        virtual ~Sized() = default;
    };

}  // namespace data_structures

namespace data_structures::list {
    template <typename T>
    struct List : Sized {
        virtual void add(const T& e) = 0;
        virtual T remove(size_t index) = 0;
        virtual T& at(size_t index) = 0;
        virtual ~List() = default;
    };

}  // namespace data_structures::list

namespace data_structures::base::tree {
    template <typename T>
    struct TreeNode {
        T data{};
        std::weak_ptr<TreeNode<T>> parent{};

        TreeNode(const T& data) : data{data} {}

        virtual bool operator==(TreeNode other) const { return this == &other; }
        virtual ~TreeNode() = default;

        virtual operator std::string() { return std::format("({})", data); }
    };

    template <typename T>
    using TreeNodePtr = shared_ptr<TreeNode<T>>;

    template <typename T>
    struct BinaryTreeNode : public TreeNode<T> {
        shared_ptr<BinaryTreeNode<T>> left{};
        shared_ptr<BinaryTreeNode<T>> right{};
        BinaryTreeNode(const T& data) : TreeNode<T>(data) {}
    };

    template <typename T>
    using BinaryNodePtr = shared_ptr<BinaryTreeNode<T>>;
}  // namespace data_structures::base::tree

namespace data_structures::tree {
    using namespace base::tree;
    template <typename T>
    struct Tree : Sized {
       protected:
        virtual std::vector<TreeNodePtr<T>> preorderSubtree(TreeNodePtr<T> n, std::vector<TreeNodePtr<T>>& snap) {
            snap.push_back(n);
            for (const auto& child : children(n)) {
                preorderSubtree(child, snap);
            }
            return snap;
        }

        virtual std::vector<TreeNodePtr<T>> postorderSubtree(TreeNodePtr<T> n, std::vector<TreeNodePtr<T>>& snap) {
            for (const auto& child : children(n)) {
                postorderSubtree(child, snap);
            }
            snap.push_back(n);
            return snap;
        }

       public:
        virtual TreeNodePtr<T> root() = 0;
        virtual std::vector<TreeNodePtr<T>> children(TreeNodePtr<T> n) = 0;
        [[nodiscard]] bool isEmpty() const override { return size() == 0; }
        virtual bool isRoot(TreeNodePtr<T> n) { return root() == n; }
        [[nodiscard]] virtual size_t numChildren(TreeNodePtr<T> n) = 0;
        virtual ~Tree() = default;

        virtual std::vector<TreeNodePtr<T>> preorder() {
            std::vector<TreeNodePtr<T>> snap{};
            if (!isEmpty()) preorderSubtree(root(), snap);
            return snap;
        }

        virtual std::vector<TreeNodePtr<T>> postorder() {
            std::vector<TreeNodePtr<T>> snap{};
            if (!isEmpty()) postorderSubtree(root(), snap);
            return snap;
        }

        size_t depth(TreeNodePtr<T> n) {
            if (isRoot(n)) return 0;
            return 1 + depth(n->parent.lock());
        }

        size_t height(TreeNodePtr<T> n) {
            size_t h{};
            for (const auto& child : children(n)) h = std::max(h, height(child) + 1);
            return h;
        }
    };

    template <typename T>
    class BinaryTree : public Tree<T> {
       protected:
        virtual std::vector<TreeNodePtr<T>> inorderSubtree(TreeNodePtr<T> n, std::vector<TreeNodePtr<T>>& snap) {
            if (!n) return snap;

            auto left = this->left(n);
            inorderSubtree(left, snap);

            snap.push_back(n);

            auto right = this->right(n);
            inorderSubtree(right, snap);

            return snap;
        }

        BinaryNodePtr<T> r;

       public:
        std::vector<TreeNodePtr<T>> children(TreeNodePtr<T> n) override {
            auto res = std::vector<TreeNodePtr<T>>{};
            if (left(n) != nullptr) res.push_back(left(n));
            if (right(n) != nullptr) res.push_back(right(n));
            return res;
        }

        TreeNodePtr<T> root() override { return r; }

        virtual BinaryNodePtr<T> left(TreeNodePtr<T> n) = 0;
        virtual BinaryNodePtr<T> right(TreeNodePtr<T> n) = 0;

        BinaryTree(T root) : r{new BinaryTreeNode<T>{root}} {}

        virtual ~BinaryTree() { r.reset(); }

        virtual std::vector<TreeNodePtr<T>> inorder() {
            std::vector<TreeNodePtr<T>> snap{};
            if (!this->isEmpty()) inorderSubtree(root(), snap);
            return snap;
        }
    };

}  // namespace data_structures::tree

namespace data_structures::graph {}

namespace data_structures::base::list {
    template <typename T>
    struct Node {
        T data{};
        shared_ptr<Node> next{nullptr};
        std::weak_ptr<Node> prev{};
        Node(const T& data) : data{data} {}
    };

    template <typename T>
    using NodePtr = shared_ptr<Node<T>>;

    template <typename T>
    struct LinkedList : public data_structures::list::List<T> {
       protected:
        size_t n{};
        shared_ptr<Node<T>> head{nullptr};

        void throwIfOutOfBoundsStrict(size_t index) {
            if (index >= n || index == std::numeric_limits<size_t>::max())
                throw std::out_of_range(std::format("Index {} is out of range", index));
        }

        void throwIfOutOfBoundsLax(size_t index) {
            if (index > n || index == std::numeric_limits<size_t>::max())
                throw std::out_of_range(std::format("Index {} is out of range", index));
        }

        virtual shared_ptr<Node<T>> getNodeAtPos(size_t pos) {
            throwIfOutOfBoundsStrict(pos);
            shared_ptr<Node<T>> curr{head};
            for (size_t i{}; i < pos; i++, curr = curr->next);
            return curr;
        }

        void checkNonEmpty() {
            if (this->isEmpty()) throw std::out_of_range("Cannot index an empty linked list");
        }

       public:
        virtual void insert(size_t index, const T& e) = 0;

        [[nodiscard]] size_t size() const override { return n; }

        operator std::string() {
            std::string str = "[";
            auto curr = head;
            for (size_t i{}; i < n; i++, curr = curr->next) {
                str += std::to_string(curr->data) + ", ";
            }
            str += "]";
            return str;
        }

        virtual ~LinkedList() {
            while (head) {
                head = head->next;
            }
        }
    };

}  // namespace data_structures::base::list

namespace data_structures::base::comparators {
    template <typename T>
    using ComparatorSignature = bool(T a, T b);

    const auto isLess = []<typename T>(T a, T b) { return a < b; };

}  // namespace data_structures::base::comparators

namespace data_structures::queue {
    using namespace data_structures::base::comparators;

    template <typename T>
    struct PriorityQueue : public Sized {
        PriorityQueue(function<ComparatorSignature<T>> comparator = isLess) : comparator{comparator} {}
        virtual void insert(const T& e) = 0;
        virtual const T& min() const = 0;
        virtual T removeMin() = 0;
        virtual ~PriorityQueue() = default;

       protected:
        void emptyPriQueue(const string& verb) const { return this->throwIfEmpty("priority queue", verb); }
        function<ComparatorSignature<T>> comparator;
    };
}  // namespace data_structures::queue

namespace data_structures::base {

    template <typename T>
    struct Stack : public Sized {
       public:
        virtual void push(const T& e) = 0;
        virtual T pop() = 0;
        virtual T& top() = 0;
        virtual ~Stack() = default;

       protected:
        void emptyStack(const string& verb) { return this->throwIfEmpty("stack", verb); }
    };

    template <typename T>
    struct Queue : public Sized {
       public:
        virtual void enqueue(const T& e) = 0;
        virtual T dequeue() = 0;
        virtual T& front() = 0;
        virtual T& back() = 0;
        virtual ~Queue() = default;

       protected:
        void emptyQueue(const string& verb) { return this->throwIfEmpty("queue", verb); }
    };

    template <typename T>
    struct Deque : public Sized {
       public:
        virtual void enqueueFront(const T& e) = 0;
        virtual void enqueueBack(const T& e) = 0;
        virtual T dequeueFront() = 0;
        virtual T dequeueBack() = 0;
        virtual T& front() = 0;
        virtual T& back() = 0;
        virtual ~Deque() = default;

       protected:
        void emptyDeque(const string& verb) { return this->throwIfEmpty("deque", verb); }
    };

}  // namespace data_structures::base
