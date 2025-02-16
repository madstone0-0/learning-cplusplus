#include <cmath>

#include "interfaces/base.hpp"
#include "list.hpp"
#pragma once

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
