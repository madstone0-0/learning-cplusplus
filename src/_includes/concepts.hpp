#pragma once

#include <bits/iterator_concepts.h>

#include <type_traits>

namespace mts {
    namespace concepts {

        template <typename T>
        concept Traversable = std::random_access_iterator<T>;

    }  // namespace concepts
}  // namespace mts
