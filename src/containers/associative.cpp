
#include <catch2/catch_all.hpp>
#include <functional>
#include <map>
#include <set>
#include <unordered_set>

/* std::Set */

using Catch::Approx;
using std::set;
TEST_CASE("std::set supports") {
    set<int> emp;
    set<int> fib{1, 1, 2, 3, 5};

    SECTION("default construction") { REQUIRE(emp.empty()); }

    SECTION("braced initialization") { REQUIRE(fib.size() == 4); }

    SECTION("copy construction") {
        auto fibCopy{fib};
        REQUIRE(fib.size() == 4);
        REQUIRE(fibCopy.size() == 4);
    }

    SECTION("move construction") {
        auto fibMove{std::move(fib)};
        REQUIRE(fib.empty());
        REQUIRE(fibMove.size() == 4);
    }

    SECTION("range construction") {
        std::array<int, 5> fibArray{1, 1, 2, 3, 5};
        std::set<int> fibSet{fibArray.cbegin(), fibArray.cend()};
        REQUIRE(fibSet.size() == 4);
    }
}

TEST_CASE("std::set allows access") {
    set<int> fib{1, 1, 2, 3, 5};
    SECTION("with find") {
        REQUIRE(*fib.find(3) == 3);
        REQUIRE(fib.find(100) == fib.end());
    }

    SECTION("with count") {
        REQUIRE(fib.count(3) == 1);
        REQUIRE(fib.count(100) == 0);
    }

    SECTION("with lower_bound") {
        auto iter = fib.lower_bound(3);
        REQUIRE(*iter == 3);
    }

    SECTION("with upper_bound") {
        auto iter = fib.upper_bound(3);
        REQUIRE(*iter == 5);
    }

    SECTION("with equal_range") {
        auto pair = fib.equal_range(3);
        REQUIRE(*pair.first == 3);
        REQUIRE(*pair.second == 5);
    }
}

TEST_CASE("std::set allows insertion") {
    set<int> fib{1, 1, 2, 3, 5};
    SECTION("with insert") {
        fib.insert(8);
        REQUIRE(fib.find(8) != fib.end());
    }

    SECTION("with emplace") {
        fib.emplace(8);
        REQUIRE(fib.find(8) != fib.end());
    }

    SECTION("with emplace_hint") {
        fib.emplace_hint(fib.end(), 8);
        REQUIRE(fib.find(8) != fib.end());
    }
}

TEST_CASE("std::set allows removal") {
    set<int> fib{1, 1, 2, 3, 5};
    SECTION("with erase") {
        fib.erase(1);
        REQUIRE(fib.find(1) == fib.end());
    }

    SECTION("with clear") {
        fib.clear();
        REQUIRE(fib.empty());
    }
}

/* std::Set */

/* std::Unordered_Set */

using std::unordered_set;
TEST_CASE("std::hash<long> returns") {
    std::hash<long> hasher;
    auto hashCode42 = hasher(42);
    SECTION("equal hash codes for equal values") { REQUIRE(hashCode42 == hasher(42)); }

    SECTION("unequal hash codes for equal values") { REQUIRE(hashCode42 != hasher(43)); }
}

TEST_CASE("std::equal_to<long> returns") {
    std::equal_to<long> longEqualTo;
    SECTION("true when arguments equal") { REQUIRE(longEqualTo(42, 42)); }
    SECTION("false when arguments unequal") { REQUIRE_FALSE(longEqualTo(42, 43)); }
}

TEST_CASE("std::unordered_set supports all set operations except for upper and lower bounds") {
    unordered_set<int> emp;
    unordered_set<int> fib{1, 1, 2, 3, 5};

    SECTION("default construction") { REQUIRE(emp.empty()); }

    SECTION("braced initialization") { REQUIRE(fib.size() == 4); }

    SECTION("copy construction") {
        auto fibCopy{fib};
        REQUIRE(fib.size() == 4);
        REQUIRE(fibCopy.size() == 4);
    }

    SECTION("move construction") {
        auto fibMove{std::move(fib)};
        REQUIRE(fib.empty());
        REQUIRE(fibMove.size() == 4);
    }

    SECTION("range construction") {
        std::array<int, 5> fibArray{1, 1, 2, 3, 5};
        unordered_set<int> fibSet{fibArray.cbegin(), fibArray.cend()};
        REQUIRE(fibSet.size() == 4);
    }
}

TEST_CASE("std::unordered_set") {
    unordered_set<unsigned long> sheep(100);
    SECTION("allowsw bucket count specification on construction") {
        REQUIRE(sheep.bucket_count() >= 100);
        REQUIRE(sheep.bucket_count() <= sheep.max_bucket_count());
        REQUIRE(sheep.max_load_factor() == Approx(1.0));
    }

    SECTION("allows us to reserve space for elements") {
        sheep.reserve(100'000);
        sheep.insert(0);
        REQUIRE(sheep.load_factor() <= 0.00001);
        while (sheep.size() < 100'000) sheep.insert(sheep.size());
        REQUIRE(sheep.load_factor() <= 1.0);
    }
}

/* std::Unordered_Set */

/* std::Map */

using std::map;

auto colorOfMagic = "Color of Magic";
auto theLightFantastic = "The Light Fantastic";
auto equalRites = "Equal Rites";
auto mort = "Mort";
TEST_CASE("std::map supports") {
    SECTION("default construction") {
        map<const char*, int> emp;
        REQUIRE(emp.empty());
    }

    SECTION("braced initialization") {
        map<const char*, int> pubYear{
            {colorOfMagic, 1983},
            {theLightFantastic, 1986},
            {equalRites, 1987},
            {mort, 1987},
        };
        REQUIRE(pubYear.size() == 4);
    }
}

TEST_CASE("std::map is an associative array with") {
    map<const char*, int> pubYear{{colorOfMagic, 1983}, {theLightFantastic, 1986}};

    SECTION("operator[]") {
        REQUIRE(pubYear[colorOfMagic] == 1983);

        pubYear[equalRites] = 1987;
        REQUIRE(pubYear[equalRites] == 1987);

        REQUIRE(pubYear[mort] == 0);
    }

    SECTION("an at method") {
        REQUIRE(pubYear.at(colorOfMagic) == 1983);
        REQUIRE_THROWS_AS(pubYear.at(equalRites), std::out_of_range);
    }
}

TEST_CASE("std::map supports insert") {
    map<const char*, int> pubYear;
    pubYear.insert({colorOfMagic, 1983});
    REQUIRE(pubYear.size() == 1);

    std::pair<const char*, int> tlfp{theLightFantastic, 1986};
    pubYear.insert(tlfp);
    REQUIRE(pubYear.size() == 2);

    auto [itr, isNew] = pubYear.insert({theLightFantastic, 9999});
    REQUIRE(itr->first == theLightFantastic);
    REQUIRE(itr->second == 1986);
    REQUIRE_FALSE(isNew);
    REQUIRE(pubYear.size() == 2);
}

TEST_CASE("std::map supports insert_or_assign") {
    map<const char*, int> pubYear{{theLightFantastic, 9999}};
    auto [itr, isNew] = pubYear.insert_or_assign(theLightFantastic, 1986);
    REQUIRE(itr->second == 1986);
    REQUIRE_FALSE(isNew);
}

TEST_CASE("We can remove std::map elements using") {
    std::map<const char*, int> pub_year{
        {colorOfMagic, 1983},
        {mort, 1987},
    };
    SECTION("erase") {
        pub_year.erase(mort);
        REQUIRE(pub_year.find(mort) == pub_year.end());
    }
    SECTION("clear") {
        pub_year.clear();
        REQUIRE(pub_year.empty());
    }
}

/* std::Map */
