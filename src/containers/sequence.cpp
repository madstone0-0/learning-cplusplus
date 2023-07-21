#define CATCH_CONFIG_MAIN
#include <array>
#include <catch2/catch.hpp>
#include <deque>
#include <list>
#include <queue>
#include <stack>
#include <vector>

/* Std::Array */

using std::array;
array<int, 10> staticArray{};
TEST_CASE("std::array") {
    REQUIRE(staticArray[0] == 0);

    SECTION("Uninitialized without braced initializers") {
        array<int, 10> localArray;
        REQUIRE(localArray[0] != 0);
    }

    SECTION("Initialized with braced initializers") {
        array<int, 10> localArray{1, 1, 2, 3};
        REQUIRE(localArray[0] == 1);
        REQUIRE(localArray[1] == 1);
        REQUIRE(localArray[2] == 2);
        REQUIRE(localArray[3] == 3);
        REQUIRE(localArray[4] == 0);
    }
}

TEST_CASE("std::array access") {
    array<int, 4> fib{1, 1, 0, 3};

    SECTION("operator[] can get and set elements") {
        fib[2] = 2;
        REQUIRE(fib[2] == 2);
        // fib[4] = 5;
    }

    SECTION("at() can get and set elements") {
        fib.at(2) = 2;
        REQUIRE(fib.at(2) == 2);
        REQUIRE_THROWS_AS(fib.at(4), std::out_of_range);
    }

    SECTION("get can get and set elements") {
        std::get<2>(fib) = 2;
        REQUIRE(std::get<2>(fib) == 2);
        // std::get<4>(fib);
    }
}

TEST_CASE("std::array has convenience methods") {
    array<int, 4> fib{0, 1, 2, 0};

    SECTION("front") {
        fib.front() = 1;
        REQUIRE(fib.front() == 1);
        REQUIRE(fib.front() == fib[0]);
    }

    SECTION("back") {
        fib.back() = 3;
        REQUIRE(fib.back() == 3);
        REQUIRE(fib.back() == fib[3]);
    }
}

// Storage Model
TEST_CASE("We can obtain a pointer to the first element in an array using") {
    array<char, 9> color{'o', 'c', 't', 'a', 'r', 'i', 'n', 'e'};
    const auto* colorPtr = color.data();
    SECTION("data") { REQUIRE(*colorPtr == 'o'); }

    SECTION("address-of front") { REQUIRE(&color.front() == colorPtr); }

    SECTION("address-of at(0)") { REQUIRE(&color.at(0) == colorPtr); }

    SECTION("address-of [0]") { REQUIRE(&color[0] == colorPtr); }
}

// Iterators
TEST_CASE("std::array begin/end form a half-opened range") {
    array<int, 0> e{};
    REQUIRE(e.begin() == e.end());
}

TEST_CASE("std::array iterators are pointer-like") {
    array<int, 3> easyAs{1, 2, 4};
    auto iter = easyAs.begin();
    REQUIRE(*iter == 1);
    iter++;
    REQUIRE(*iter == 2);
    iter++;
    REQUIRE(*iter == 4);
    iter++;
    REQUIRE(iter == easyAs.end());
}

TEST_CASE("std::array can be used as a range expression") {
    array<int, 5> fib{1, 1, 2, 3, 5};
    int sum{};

    for (const auto element : fib) {
        sum += element;
    }

    REQUIRE(sum == 12);
}

/* Std::Array */

/* std::Vector */

using std::vector;
TEST_CASE("std::vector supports default construction") {
    vector<const char*> vec;
    REQUIRE(vec.empty());
}

TEST_CASE("std::vector supportedd braced initialization") {
    vector<int> fib{1, 1, 2, 3, 5};
    REQUIRE(fib[4] == 5);
}

TEST_CASE("std::vector supports") {
    SECTION("braced initialization") {
        vector<int> fiveNine{5, 9};
        REQUIRE(fiveNine[0] == 5);
        REQUIRE(fiveNine[1] == 9);
    }

    SECTION("fill contructor") {
        vector<int> fiveNines(5, 9);
        REQUIRE(fiveNines[0] == 9);
        REQUIRE(fiveNines[4] == 9);
    }
}

TEST_CASE("std::vector supports construction from iterators") {
    array<int, 5> fibArr{1, 1, 2, 3, 5};
    // vector<int> fibVec(fibArr.begin(), fibArr.end());
    vector<int> fibVec(fibArr.begin(), fibArr.end());
    REQUIRE(fibVec[4] == 5);
    REQUIRE(fibVec.size() == fibArr.size());
}

TEST_CASE("std::vector assign replaces existing elements") {
    vector<int> message{13, 80, 110, 114, 102, 110, 101};
    REQUIRE(message.size() == 7);
    message.assign({67, 97, 101, 115, 97, 114});
    REQUIRE(message[5] == 114);
    REQUIRE(message.size() == 6);
}

TEST_CASE("std::vector insert places new elements") {
    vector<int> zeros(3, 0);
    auto third = zeros.begin() + 2;
    zeros.insert(third, 10);
    REQUIRE(zeros[2] == 10);
    REQUIRE(zeros.size() == 4);
}

TEST_CASE("std::vector push_back places new elements") {
    vector<int> zeros(3, 0);
    zeros.push_back(10);
    REQUIRE(zeros[3] == 10);
}

TEST_CASE("std::vector emplace methods forward arguments") {
    vector<std::pair<int, int>> factors;
    factors.emplace_back(2, 20);
    factors.emplace_back(3, 20);
    factors.emplace_back(4, 15);
    factors.emplace(factors.begin(), 1, 60);
    REQUIRE(factors[0].first == 1);
    REQUIRE(factors[0].second == 60);
    REQUIRE(factors.back().first == 4);
    REQUIRE(factors.back().second == 15);
}

TEST_CASE("std::vector exposes size management methods") {
    vector<array<uint8_t, 1024>> kbStore;
    REQUIRE(kbStore.max_size() > 0);
    REQUIRE(kbStore.empty());

    size_t elements{1024};
    kbStore.reserve(elements);

    REQUIRE(kbStore.empty());
    REQUIRE(kbStore.capacity() == elements);

    kbStore.emplace_back();
    kbStore.emplace_back();
    kbStore.emplace_back();
    REQUIRE(kbStore.size() == 3);

    kbStore.clear();
    REQUIRE(kbStore.empty());
    REQUIRE(kbStore.capacity() >= 3);
}

/* std::Vector */

/* std::Deque */

using std::deque;
TEST_CASE("std::deque supports front insertion") {
    deque<char> deckard;
    deckard.push_front('a');
    deckard.push_back('i');
    deckard.push_front('c');
    deckard.push_back('n');

    REQUIRE(deckard[0] == 'c');
    REQUIRE(deckard[1] == 'a');
    REQUIRE(deckard[2] == 'i');
    REQUIRE(deckard[3] == 'n');
}

/* std::Deque */

/* std::List */

using std::list;
TEST_CASE("std::list supports insertion") {
    list<int> odds{11, 22, 33, 44, 55};
    odds.remove_if([](int x) { return x % 2 == 0; });
    auto oddsIter = odds.begin();
    REQUIRE(*oddsIter == 11);
    oddsIter++;
    REQUIRE(*oddsIter == 33);
    oddsIter++;
    REQUIRE(*oddsIter == 55);
    oddsIter++;
    REQUIRE(oddsIter == odds.end());
}

/* std::List */

/* std::Stack */

using std::stack;
TEST_CASE("std::stack supports push/pop/top ops") {
    vector<int> vec{1, 3};
    stack<int, decltype(vec)> easyAs{vec};
    REQUIRE(easyAs.top() == 3);
    easyAs.pop();
    easyAs.push(2);
    REQUIRE(easyAs.top() == 2);
    easyAs.pop();
    REQUIRE(easyAs.top() == 1);
    easyAs.pop();
    REQUIRE(easyAs.empty());
}

/* std::Stack */

/* std::Queue */

using std::queue;
TEST_CASE("std::queue supports push/pop/front/back ops") {
    deque<int> deq{1, 2};
    queue<int> easyAs(deq);

    REQUIRE(easyAs.front() == 1);
    REQUIRE(easyAs.back() == 2);
    easyAs.pop();
    easyAs.push(3);
    REQUIRE(easyAs.front() == 2);
    REQUIRE(easyAs.back() == 3);
    easyAs.pop();
    REQUIRE(easyAs.front() == 3);
    easyAs.pop();
    REQUIRE(easyAs.empty());
}

/* std::Queue */

/* std::Priority_Queue */

using std::priority_queue;
TEST_CASE("std::priority_queue supports push/pop") {
    priority_queue<double> prique;
    prique.push(1.0);
    prique.push(2.0);
    prique.push(1.5);

    REQUIRE(prique.top() == Approx(2.0));
    prique.pop();
    prique.push(1.0);
    REQUIRE(prique.top() == Approx(1.5));
    prique.pop();
    REQUIRE(prique.top() == Approx(1.0));
    prique.pop();
    REQUIRE(prique.top() == Approx(1.0));
    prique.pop();
    REQUIRE(prique.empty());
}

/* std::Priority_Queue */
