#pragma once
#include "base.hpp"

namespace  data_structures::heap {
    template <typename T>
    struct Heap : public Sized {
        virtual void insert(const T& e) = 0;
        virtual T& extreme() = 0;
        virtual T extractExtreme() = 0;
        virtual ~Heap() = default;

       protected:
        void emptyHeap(const string& verb) { return this->throwIfEmpty("heap", verb); }
    };
}

