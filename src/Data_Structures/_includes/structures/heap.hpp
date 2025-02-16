#pragma once
#include <cmath>

#include "interfaces/heap.hpp"
#include "list.hpp"

namespace data_structures::heap {
    template <typename T>
    class ArrayMinHeap : public Heap<T> {
       public:
        void insert(const T& e) override {
            backing.add(e);
            size_t nodeIdx{size() - 1};
            while (nodeIdx > 0) {
                size_t parentIdx = std::floor((nodeIdx - 1) / 2);
                if (backing.at(parentIdx) <= backing.at(nodeIdx)) break;
                std::swap(backing.at(nodeIdx), backing.at(parentIdx));
                nodeIdx = parentIdx;
            }
        }

        T& extreme() override { return backing.at(0); }

        T extractExtreme() override {
            std::swap(backing.at(0), backing.at(backing.size() - 1));
            T extreme = backing.remove(backing.size() - 1);
            size_t nodeIdx{};
            while (nodeIdx < size()) {
                size_t leftIdx = 2 * nodeIdx + 1;
                size_t rightIdx = 2 * nodeIdx + 2;
                size_t smallerIdx{nodeIdx};

                if (leftIdx > size() - 1) break;
                if (backing.at(leftIdx) < backing.at(smallerIdx)) smallerIdx = leftIdx;
                if (rightIdx < size() && backing.at(rightIdx) < backing.at(smallerIdx)) smallerIdx = rightIdx;

                if (smallerIdx == nodeIdx) break;
                std::swap(backing.at(smallerIdx), backing.at(nodeIdx));
                nodeIdx = smallerIdx;
            }
            return extreme;
        }

        [[nodiscard]] size_t size() const override { return backing.size(); }

       private:
        data_structures::list::ArrayList<T> backing;
    };

    template <typename T>
    class ArrayMaxHeap : public Heap<T> {
       public:
        void insert(const T& e) override {
            backing.add(e);
            size_t nodeIdx{size() - 1};
            while (nodeIdx > 0) {
                size_t parentIdx = std::floor((nodeIdx - 1) / 2);
                if (backing.at(parentIdx) >= backing.at(nodeIdx)) break;
                std::swap(backing.at(nodeIdx), backing.at(parentIdx));
                nodeIdx = parentIdx;
            }
        }

        T& extreme() override { return backing.at(0); }

        T extractExtreme() override {
            std::swap(backing.at(0), backing.at(backing.size() - 1));
            T extreme = backing.remove(backing.size() - 1);
            size_t nodeIdx{};
            while (nodeIdx < size()) {
                size_t leftIdx = 2 * nodeIdx + 1;
                size_t rightIdx = 2 * nodeIdx + 2;
                size_t largerIdx{nodeIdx};

                if (leftIdx > size() - 1) break;
                if (backing.at(leftIdx) > backing.at(largerIdx)) largerIdx = leftIdx;
                if (rightIdx < size() && backing.at(rightIdx) > backing.at(largerIdx)) largerIdx = rightIdx;

                if (largerIdx == nodeIdx) break;
                std::swap(backing.at(largerIdx), backing.at(nodeIdx));
                nodeIdx = largerIdx;
            }
            return extreme;
        }

        [[nodiscard]] size_t size() const override { return backing.size(); }

       private:
        data_structures::list::ArrayList<T> backing;
    };
}  // namespace data_structures::heap
