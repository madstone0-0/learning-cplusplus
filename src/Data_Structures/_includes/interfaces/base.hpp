#pragma once
#include <functional>
#include <memory>

namespace data_structures {
    namespace rg = std::ranges;
    using std::shared_ptr, std::string, std::unique_ptr, std::function;
    struct Sized {
       public:
        [[nodiscard]] virtual size_t size() const = 0;
        [[nodiscard]] virtual bool isEmpty() const { return size() == 0; }
        void throwIfEmpty(const string& object, const string& verb) const {
            if (isEmpty()) throw std::runtime_error(std::format("Cannot {} from an empty {}", verb, object));
        }

        virtual ~Sized() = default;
    };
}  // namespace data_structures

namespace data_structures::base::comparators {
    template <typename T>
    using ComparatorSignature = bool(T a, T b);

    const auto isLess = []<typename T>(T a, T b) { return a < b; };

}  // namespace data_structures::base::comparators

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
