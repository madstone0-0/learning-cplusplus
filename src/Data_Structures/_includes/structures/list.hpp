#pragma once
#include "interfaces/list.hpp"

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

            while (i < size_ - 1) {
                array[i] = array[i + 1];
                i++;
            }
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
