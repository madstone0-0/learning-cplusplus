#pragma once
#include "base.hpp"

namespace data_structures::list {
    template <typename T>
    struct List : Sized {
        virtual void add(const T& e) = 0;
        virtual T remove(size_t index) = 0;
        virtual T& at(size_t index) = 0;
        virtual ~List() = default;
    };

}  // namespace data_structures::list

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
