#pragma once
#include "interfaces/queue.hpp"
#include "structures/list.hpp"

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
