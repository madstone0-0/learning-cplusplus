#include <cstddef>
#include <format>
#include <list>
#include <memory>
#include <stdexcept>
#include <utility>

#include "fundamental_interfaces.hpp"
#include "utils.hpp"

using std::shared_ptr, std::unique_ptr, std::string;

namespace data_structures::list {
    template <typename T>
    class ArrayList : public List<T> {
       public:
        const static int INIT_SIZE{10};

        ArrayList() : List<T>{}, MIN_SIZE{INIT_SIZE}, array{new T[INIT_SIZE]} {}

        ArrayList(const ArrayList& other) : MIN_SIZE{other.MIN_SIZE}, size_{other.size_}, array{new T[other.size()]} {
            std::copy(other.array.get(), other.array.get() + other.size(), array.get());
        }

        ArrayList(ArrayList&& other) noexcept
            : MIN_SIZE{other.MIN_SIZE}, size_{other.size_}, array{std::move(other.array)} {
            other.size_ = 0;
            other.MIN_SIZE = INIT_SIZE;
        }

        ArrayList& operator=(ArrayList other) {
            swap(other, *this);
            return *this;
        }

        ArrayList& operator=(ArrayList&& other) noexcept {
            if (&other == this) return *this;
            MIN_SIZE = std::move(other.MIN_SIZE);
            size_ = other.size_;
            array = std::move(other.array);
            other.size_ = 0;
            other.MIN_SIZE = INIT_SIZE;

            return *this;
        }

        ~ArrayList() override { array.reset(); }

        void add(const T& e) override {
            increaseIfFull();
            array[size_] = e;
            size_++;
        }

        [[nodiscard]] bool isEmpty() const override { return size_ == 0; }

        T remove(size_t index) override {
            throwIfOutOfBounds(index);
            T removed = array[index];
            size_t i{index};

            while (i < size_) {
                array[i] = array[i + 1];
                i++;
            }
            array[i] = 0;
            size_--;
            return removed;
        }

        T& at(size_t index) override {
            throwIfOutOfBounds(index);
            return array[index];
        }

        [[nodiscard]] size_t size() const override { return size_; }

        operator std::string() {
            std::string str = "[";
            for (size_t i{}; i < size_; i++) {
                str += std::format("{}, ", array[i]);
            }
            str += "]";
            return str;
        }

       private:
        friend void swap(ArrayList<T>& first, ArrayList<T>& second) {
            using std::swap;

            swap(first.MIN_SIZE, second.MIN_SIZE);
            swap(first.array, second.array);
        }

        void throwIfOutOfBounds(size_t index) {
            if (index >= size_ || size_ < 0) throw std::out_of_range(std::format("Index {} is out of range", index));
        }

        void increaseIfFull() {
            if (size_ == MIN_SIZE) {
                MIN_SIZE *= 2;
                unique_ptr<T[]> newArray = std::make_unique<T[]>(MIN_SIZE);
                for (size_t i{}; i < size_; i++) newArray[i] = array[i];
                array = std::move(newArray);
            }
        }

        size_t size_{};
        size_t MIN_SIZE;
        unique_ptr<T[]> array;
    };

    template <typename T>
    class LinkedList : public List<T> {
       public:
        LinkedList() : List<T>{} {}

        LinkedList(const LinkedList& other) : n{other.n} {
            auto curr = other.head;
            head = std::make_shared<Node>(curr->data);
            auto temp = head;
            curr = curr->next;
            for (size_t i{1}; i < n; i++, curr = curr->next, temp = temp->next) {
                temp->next = std::make_shared<Node>(curr->data);
                temp->next->prev = temp;
            }
            tail = temp;
        }

        LinkedList(LinkedList&& other) noexcept : n{other.n}, head{std::move(other.head)}, tail{std::move(other.tail)} {
            other.n = 0;
        }

        LinkedList& operator=(LinkedList other) {
            swap(other, *this);
            return *this;
        }

        LinkedList& operator=(LinkedList&& other) noexcept {
            if (&other == this) return *this;
            head = std::move(other.head);
            tail = std::move(other.tail);
            n = other.n;

            other.n = 0;
            return *this;
        }

        ~LinkedList() override {
            while (head) {
                head->prev = nullptr;
                head = head->next;
            }
            tail = nullptr;
        }

        void add(const T& e) override {
            if (head == nullptr) {
                head = std::make_shared<Node>(e);
                tail = head;
                n++;
                return;
            }

            tail->next = std::make_shared<Node>(e);
            tail->next->prev = tail;
            tail = tail->next;
            n++;
        }

        [[nodiscard]] bool isEmpty() const override { return n == 0; }

        T remove(size_t index) override {
            throwIfOutOfBounds(index);

            if (index == 0 && n == 1) {
                auto res = head->data;
                head = nullptr;
                tail = nullptr;
                tail = head;
                n--;
                return res;
            }

            if (index == 0) {
                auto res = head->data;
                head = head->next;
                head->prev = nullptr;
                n--;
                return res;
            }

            if (index == n - 1) {
                auto res = tail->data;
                tail = tail->prev;
                tail->next = nullptr;
                n--;
                return res;
            }

            auto prev = getNodeAtPos(index - 1);
            auto res = prev->next->data;
            prev->next = prev->next->next;
            prev->next->prev = prev;
            n--;
            return res;
        }

        T& at(size_t index) override {
            throwIfOutOfBounds(index);
            return getNodeAtPos(index)->data;
        }

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

       private:
        struct Node {
            T data{};
            shared_ptr<Node> next{};
            shared_ptr<Node> prev{};

            Node(const T& data) : data{data} {}
        };

        friend void swap(LinkedList<T>& first, LinkedList<T>& second) {
            using std::swap;

            swap(first.n, second.n);
            swap(first.head, second.head);
            swap(first.tail, second.tail);
        }

        void throwIfOutOfBounds(size_t index) {
            if (index >= n || n < 0) throw std::out_of_range(std::format("Index {} is out of range", index));
        }

        shared_ptr<Node> getNodeAtPos(size_t pos) {
            throwIfOutOfBounds(pos);
            shared_ptr<Node> curr{};
            if (pos > n / 2) {
                curr = tail;
                for (size_t i{n - 1}; i > pos; i--, curr = curr->prev);
            } else {
                curr = head;
                for (size_t i{}; i < pos; i++, curr = curr->next);
            }
            return curr;
        }

        size_t n{};
        shared_ptr<Node> head{nullptr};
        shared_ptr<Node> tail{head};
    };

}  // namespace data_structures::list

namespace data_structures::tree {
    template <typename T>
    class LinkedBinaryTree : public BinaryTree<T> {
       public:
        LinkedBinaryTree(const T& rootVal) : BinaryTree<T>(rootVal), n{1} {}

        BinaryNodePtr<T> validate(TreeNodePtr<T> n) {
            if (!Utils:: instanceof <BinaryTreeNode<T>>(n.get())) throw std::invalid_argument("Invalid node");
            return std::dynamic_pointer_cast<BinaryTreeNode<T>>(n);
        }

        [[nodiscard]] size_t size() const override { return n; }

        BinaryNodePtr<T> left(TreeNodePtr<T> n) override {
            auto node = validate(n);
            return node->left;
        }

        BinaryNodePtr<T> right(TreeNodePtr<T> n) override {
            auto node = validate(n);
            return node->right;
        }

        BinaryNodePtr<T> addLeft(TreeNodePtr<T> n, const T& val) {
            auto parent = validate(n);
            if (parent->left != nullptr) throw std::invalid_argument("Node already has left");
            parent->left = std::make_shared<BinaryTreeNode<T>>(val);
            parent->left->parent = parent;
            this->n++;
            return parent->left;
        }

        BinaryNodePtr<T> addRight(TreeNodePtr<T> n, const T& val) {
            auto parent = validate(n);
            if (parent->right != nullptr) throw std::invalid_argument("Node already has right");
            parent->right = std::make_shared<BinaryTreeNode<T>>(val);
            parent->right->parent = parent;
            this->n++;
            return parent->right;
        }

        size_t numChildren(TreeNodePtr<T> n) override {
            auto node = validate(n);
            size_t num{};
            if (left(node) != nullptr) num++;
            if (right(node) != nullptr) num++;
            return num;
        }

        T remove(TreeNodePtr<T> n) {
            if (this->isRoot(n)) {
                throw std::invalid_argument("Cannot remove root");
            }

            BinaryNodePtr<T> node = validate(n);
            auto res = node->data;
            auto child = node->left != nullptr ? node->left : node->right;
            if (child != nullptr) child->parent = node->parent.lock();
            if (child == this->r)
                this->r = child;
            else {
                auto parent = validate(node->parent.lock());
                if (node == parent->left)
                    parent->left = child;
                else
                    parent->right = child;
            }
            this->n--;
            node.reset();
            return res;
        }

        void preorderDisplay(TreeNodePtr<T> n, string& out, const string& pre, const string& childPre) {
            auto node = validate(n);
            out += pre;
            out += node->data;
            out += "\n";
            auto kinder = this->children(node);
            for (size_t i{}; i < kinder.size(); i++) {
                auto child = kinder.at(i);
                if (i < kinder.size() - 1)
                    preorderDisplay(child, out, childPre + "├── ", childPre + "│   ");
                else
                    preorderDisplay(child, out, childPre + "└── ", childPre + "    ");
            }
        }

        string preorderDisplay() {
            string out{};
            preorderDisplay(this->root(), out, "", "");
            return out;
        }

       private:
        size_t n{};
    };
}  // namespace data_structures::tree

namespace data_structures::base::list {
    template <typename T>
    class DoublyLinkedList : public LinkedList<T> {
       public:
        DoublyLinkedList() : LinkedList<T>{} {}

        DoublyLinkedList(const DoublyLinkedList& other) : LinkedList<T>{} {
            this->n = other.n;
            auto curr = other.head;
            this->head = std::make_shared<Node<T>>(curr->data);
            auto temp = this->head;
            curr = curr->next;
            for (size_t i{1}; i < this->n; i++, curr = curr->next, temp = temp->next) {
                temp->next = std::make_shared<Node<T>>(curr->data);
                temp->next->prev = temp;
            }
            tail = temp;
        }

        DoublyLinkedList(DoublyLinkedList&& other) noexcept : LinkedList<T>{} {
            this->n = other.n;
            this->head = std::move(other.head);
            this->tail = std::move(other.tail);

            other.n = 0;
        }

        DoublyLinkedList& operator=(DoublyLinkedList other) {
            swap(other, *this);
            return *this;
        }

        DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
            if (&other == this) return *this;
            this->n = other.n;
            this->head = std::move(other.head);
            this->tail = std::move(other.tail);

            other.n = 0;
            return *this;
        }

        ~DoublyLinkedList() override {
            while (this->head) {
                this->head = this->head->next;
            }
            tail = nullptr;
        }

        void add(const T& e) override {
            if (this->head == nullptr) {
                this->head = std::make_shared<Node<T>>(e);
                tail = this->head;
                this->n++;
                return;
            }

            tail->next = std::make_shared<Node<T>>(e);
            tail->next->prev = tail;
            tail = tail->next;
            this->n++;
        }

        [[nodiscard]] bool isEmpty() const override { return this->n == 0; }

        NodePtr<T> getNodeAtPos(size_t pos) override {
            this->throwIfOutOfBoundsStrict(pos);
            NodePtr<T> curr{};
            if (pos > this->n / 2) {
                curr = tail;
                for (size_t i{this->n - 1}; i > pos; i--, curr = curr->prev.lock());
            } else {
                curr = this->head;
                for (size_t i{}; i < pos; i++, curr = curr->next);
            }
            return curr;
        }

        T remove(size_t index) override {
            this->checkNonEmpty();
            this->throwIfOutOfBoundsStrict(index);

            if (index == 0 && this->n == 1) {
                auto res = this->head->data;
                this->head = nullptr;
                tail = nullptr;
                tail = this->head;
                this->n--;
                return res;
            }

            if (index == 0) {
                auto res = this->head->data;
                this->head = this->head->next;
                if (this->head) this->head->prev.reset();
                this->n--;
                return res;
            }

            if (index == this->n - 1) {
                auto res = tail->data;
                tail = tail->prev.lock();
                if (tail) tail->next = nullptr;
                this->n--;
                return res;
            }

            auto prev = getNodeAtPos(index - 1);
            auto res = prev->next->data;
            prev->next = prev->next->next;
            if (prev->next) prev->next->prev = prev;
            this->n--;
            return res;
        }

        void insert(size_t index, const T& e) override {
            this->throwIfOutOfBoundsLax(index);

            if (this->n == 0) {
                if (index == 0) {
                    this->head = std::make_shared<Node<T>>(e);
                    tail = this->head;
                    this->n++;
                    return;
                } else {
                    throw std::out_of_range("Cannot insert on an empty linked list at a non-zero index");
                }
            }

            if (index == 0) {
                auto node = std::make_shared<Node<T>>(e);
                node->next = this->head;
                this->head->prev = node;
                this->head = node;
                this->n++;
                return;
            }

            if (index == this->n) return add(e);

            auto prev = getNodeAtPos(index - 1);
            auto next = prev->next;

            prev->next = std::make_shared<Node<T>>(e);
            prev->next->prev = prev;

            prev->next->next = next;
            next->prev = prev->next;
            this->n++;
        }

        T& at(size_t index) override {
            this->checkNonEmpty();
            this->throwIfOutOfBoundsStrict(index);
            return getNodeAtPos(index)->data;
        }

        const NodePtr<T>& gHead() const { return this->head; }

        const NodePtr<T>& gTail() const { return tail; }

       private:
        friend void swap(DoublyLinkedList<T>& first, DoublyLinkedList<T>& second) {
            using std::swap;

            swap(first.n, second.n);
            swap(first.head, second.head);
            swap(first.tail, second.tail);
        }

        NodePtr<T> tail{this->head};
    };

    template <typename T>
    class SinglyLinkedList : public LinkedList<T> {
       public:
        SinglyLinkedList() : LinkedList<T>{} {}

        SinglyLinkedList(const SinglyLinkedList& other) {
            this->n = other.n;
            auto curr = other.head;
            this->head = std::make_shared<Node<T>>(curr->data);
            auto temp = this->head;
            curr = curr->next;
            for (size_t i{1}; i < this->n; i++, curr = curr->next, temp = temp->next)
                temp->next = std::make_shared<Node<T>>(curr->data);
        }

        SinglyLinkedList(SinglyLinkedList&& other) noexcept {
            this->n = other.n;
            this->head = std::move(other.head);

            other.n = 0;
        }

        SinglyLinkedList& operator=(SinglyLinkedList other) {
            swap(other, *this);
            return *this;
        }

        SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept {
            if (&other == this) return *this;
            this->n = other.n;
            this->head = std::move(other.head);

            other.n = 0;
            return *this;
        }

        ~SinglyLinkedList() override {
            while (this->head) this->head = this->head->next;
        }

        void add(const T& e) override {
            if (this->n == 0) {
                this->head = std::make_shared<Node<T>>(e);
                this->n++;
                return;
            }

            auto curr = this->head;
            while (curr->next != nullptr) curr = curr->next;

            curr->next = std::make_shared<Node<T>>(e);
            this->n++;
        }

        T remove(size_t index) override {
            this->checkNonEmpty();
            this->throwIfOutOfBoundsStrict(index);

            if (index == 0) {
                auto res = this->head->data;
                this->head = this->head->next;
                this->n--;
                return res;
            }

            auto prev = this->getNodeAtPos(index - 1);
            auto res = prev->next->data;
            prev->next = prev->next->next;
            this->n--;
            return res;
        }

        T& at(size_t index) override {
            this->checkNonEmpty();
            this->throwIfOutOfBoundsStrict(index);

            return this->getNodeAtPos(index)->data;
        }

        [[nodiscard]] bool isEmpty() const override { return this->n == 0; }

        void insert(size_t index, const T& e) override {
            this->throwIfOutOfBoundsLax(index);

            if (this->n == 0) {
                if (index == 0) {
                    this->head = std::make_shared<Node<T>>(e);
                    this->n++;
                    return;
                } else {
                    throw std::out_of_range("Cannot insert on an empty linked list at a non-zero index");
                }
            }

            if (index == 0) {
                auto node = std::make_shared<Node<T>>(e);
                node->next = this->head;
                this->head = node;
                this->n++;
                return;
            }

            if (index == this->n) return add(e);

            auto prev = this->getNodeAtPos(index - 1);
            auto next = prev->next;
            prev->next = std::make_shared<Node<T>>(e);
            prev->next->next = next;
            this->n++;
        }

       private:
        void swap(SinglyLinkedList<T>& first, SinglyLinkedList<T>& second) {
            using std::swap;
            swap(first.n, second.n);
            swap(first.head, second.head);
        }
    };
}  // namespace data_structures::base::list

namespace data_structures::base {
    template <typename T>
    class LinkedStack : public Stack<T> {
       public:
        LinkedStack() : Stack<T>{} {}

        LinkedStack(const LinkedStack& other) : Stack<T>{}, backing{other.backing} {}

        LinkedStack(LinkedStack&& other) noexcept : Stack<T>{}, backing{std::move(other.backing)} {}

        LinkedStack& operator=(LinkedStack other) {
            swap(other, *this);
            return *this;
        }

        LinkedStack& operator=(LinkedStack&& other) noexcept { backing = std::move(other.backing); }

        ~LinkedStack() = default;

        void push(const T& e) override {
            if (isEmpty()) {
                backing.add(e);
                return;
            }
            backing.insert(0, e);
        }

        T pop() override {
            this->emptyStack("pop");
            return backing.remove(0);
        }

        T& top() override {
            this->emptyStack("get top");
            return backing.at(0);
        }

        [[nodiscard]] size_t size() const override { return backing.size(); }

        [[nodiscard]] bool isEmpty() const override { return backing.isEmpty(); }

       private:
        friend void swap(LinkedStack<T>& first, LinkedStack<T>& second) { std::swap(first.backing, second.backing); }

        list::SinglyLinkedList<T> backing{};
    };

    template <typename T>
    class LinkedQueue : public Queue<T> {
       public:
        LinkedQueue() : Queue<T>{} {}

        LinkedQueue(const LinkedQueue& other) : Queue<T>{}, backing{other.backing} {}

        LinkedQueue(LinkedQueue&& other) noexcept : Queue<T>{}, backing{std::move(other.backing)} {}

        LinkedQueue& operator=(LinkedQueue other) {
            swap(other, *this);
            return *this;
        }

        LinkedQueue& operator=(LinkedQueue&& other) noexcept {
            if (&other == this) return *this;
            backing = std::move(other.backing);
            return *this;
        }

        ~LinkedQueue() = default;

        void enqueue(const T& e) override { backing.add(e); }

        T dequeue() override {
            this->emptyQueue("dequeue");
            return backing.remove(0);
        }

        T& front() override {
            this->emptyQueue("get front");
            return backing.at(0);
        }

        T& back() override {
            this->emptyQueue("get back");
            return backing.gTail()->data;
        }

        [[nodiscard]] size_t size() const override { return backing.size(); }

        [[nodiscard]] bool isEmpty() const override { return backing.isEmpty(); }

       private:
        friend void swap(LinkedQueue<T>& first, LinkedQueue<T>& second) { std::swap(first.backing, second.backing); }
        list::DoublyLinkedList<T> backing;
    };

    template <typename T>
    class LinkedDeque : public Deque<T> {
       public:
        LinkedDeque() : Deque<T>{} {}

        LinkedDeque(const LinkedDeque& other) : backing{other.backing} {}

        LinkedDeque(LinkedDeque&& other) noexcept : backing{std::move(other.backing)} {}

        LinkedDeque& operator=(LinkedDeque other) {
            swap(other, *this);
            return *this;
        }

        LinkedDeque& operator=(LinkedDeque&& other) noexcept {
            if (this == &other) return *this;
            backing = std::move(other.backing);
            return *this;
        }

        void enqueueFront(const T& e) override { backing.insert(0, e); }

        void enqueueBack(const T& e) override { backing.add(e); }

        T dequeueFront() override {
            this->emptyDeque("dequeue");
            return backing.remove(0);
        }

        T dequeueBack() override {
            this->emptyDeque("dequeue");
            return backing.remove(backing.size() - 1);
        }

        T& front() override {
            this->emptyDeque("get front");
            return backing.at(0);
        }

        T& back() override {
            this->emptyDeque("get back");
            return backing.gTail()->data;
        }

        [[nodiscard]] size_t size() const override { return backing.size(); }

        [[nodiscard]] bool isEmpty() const override { return backing.isEmpty(); }

        ~LinkedDeque() = default;

       private:
        void swap(LinkedDeque<T>& first, LinkedDeque<T>& second) { std::swap(first.backing, second.backing); }

        list::DoublyLinkedList<T> backing{};
    };
}  // namespace data_structures::base

namespace data_structures::queue {
    template <typename T>
    class LinkedPriorityQueue : public PriorityQueue<T> {
       public:
        LinkedPriorityQueue() : PriorityQueue<T>{} {}

        LinkedPriorityQueue(const LinkedPriorityQueue& other)
            : PriorityQueue<T>{other.comparator}, backing{other.backing} {}

        LinkedPriorityQueue(LinkedPriorityQueue&& other) noexcept
            : PriorityQueue<T>{std::move(other.comparator)}, backing{std::move(other.backing)} {}

        LinkedPriorityQueue& operator=(LinkedPriorityQueue other) {
            swap(*this, other);
            return *this;
        }

        LinkedPriorityQueue& operator=(LinkedPriorityQueue&& other) noexcept {
            if (&other == this) return *this;
            swap(*this, other);
            return *this;
        }

        LinkedPriorityQueue(function<ComparatorSignature<T>> comparator) : PriorityQueue<T>{comparator} {}

        void insert(const T& e) override {
            if (isEmpty()) return backing.add(e);
            return backing.insert(findLargerIndex(e), e);
        }

        const T& min() const override {
            this->emptyPriQueue("get min");
            return backing.gHead()->data;
        }

        T removeMin() override {
            this->emptyPriQueue("remove min");
            return backing.remove(0);
        }

        [[nodiscard]] size_t size() const override { return backing.size(); }

        [[nodiscard]] bool isEmpty() const override { return backing.isEmpty(); }

        ~LinkedPriorityQueue() override = default;

       private:
        size_t findLargerIndex(const T& e) const {
            auto curr = backing.gHead();
            for (size_t i{}; i < backing.size(); i++, curr = curr->next) {
                if (this->comparator(e, curr->data)) return i;
            }
            return backing.size();
        }

        void swap(LinkedPriorityQueue<T>& first, LinkedPriorityQueue<T>& second) {
            std::swap(first.backing, second.backing);
        }

        base::list::DoublyLinkedList<T> backing{};
    };
}  // namespace data_structures::queue
