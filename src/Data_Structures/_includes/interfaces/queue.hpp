#pragma once
#include "base.hpp"

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
