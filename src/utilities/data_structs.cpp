
#include <any>
#include <boost/logic/tribool.hpp>
#include <catch2/catch_all.hpp>
#include <optional>
#include <tuple>
#include <utility>
#include <variant>

/* Tribool */
using namespace Catch;
using boost::logic::indeterminate;
boost::logic::tribool t = true, f = false, i = indeterminate;
TEST_CASE("Boost tribool converts to bool") {
    REQUIRE(t.value);
    REQUIRE_FALSE(f.value);
    REQUIRE(!f.value);
    REQUIRE_FALSE(!t.value);
    REQUIRE(indeterminate(i));
    REQUIRE_FALSE(indeterminate(t));
}

TEST_CASE("Boost Tribool supports Boolean operations") {
    auto t_or_f = t || f;
    REQUIRE(t_or_f.value);
    REQUIRE(indeterminate(t && indeterminate));
    REQUIRE(indeterminate(f || indeterminate));
    REQUIRE(indeterminate(!i));
}

TEST_CASE("Boost Tribool works nicely with if statements") {
    if (i)
        FAIL("Indeterminate is true.");
    else if (!i)
        FAIL("Indeterminate is false.");
    else {
        printf("Water on the carpet something spilled in total darkness\n");
    }  // OK, indeterminate
}

/* Tribool */

/* Optional */

struct TheMatrix {
    TheMatrix(int x) : iteration{x} {}
    const int iteration;
};

enum Pill { Red, Blue };

std::optional<TheMatrix> take(Pill pill) {
    if (pill == Pill::Blue) return TheMatrix{6};
    return std::nullopt;
}

TEST_CASE("std::optional contains types") {
    if (auto maxtrixOpt = take(Pill::Blue)) {
        REQUIRE(maxtrixOpt->iteration == 6);
        auto& matrix = maxtrixOpt.value();
        REQUIRE(matrix.iteration == 6);
    } else {
        FAIL("The optional evaluated to false");
    }
}

TEST_CASE("std::optional can be empty") {
    auto matrix_opt = take(Pill::Red);
    if (matrix_opt) FAIL("The Matrix is not empty.");
    REQUIRE_FALSE(matrix_opt.has_value());
}

/* Optional */

/* Pair */
struct Socialite {
    const char* birthname;
};

struct Valet {
    const char* surname;
};

Socialite bertie{"Wilberforce"};
Valet reginald{"Jeeves"};

TEST_CASE("std::pair permits access to memebers") {
    std::pair<Socialite, Valet> immutableDuo{bertie, reginald};
    REQUIRE(immutableDuo.first.birthname == bertie.birthname);
    REQUIRE(immutableDuo.second.surname == reginald.surname);
}

TEST_CASE("std::pair works with structured binding") {
    std::pair<Socialite, Valet> immutableDuo{bertie, reginald};
    auto& [idleRich, butler] = immutableDuo;
    REQUIRE(idleRich.birthname == bertie.birthname);
    REQUIRE(butler.surname == reginald.surname);
}
/* Pair */

/* Tuple */

struct Acquaintance {
    const char* nickname;
};

Acquaintance hildebrand{"Tuppy"};

TEST_CASE("std::tuple permits access to members with std::get") {
    using Trio = std::tuple<Socialite, Valet, Acquaintance>;
    Trio truculentTrio{bertie, reginald, hildebrand};
    auto& bertieRef = std::get<0>(truculentTrio);
    REQUIRE(bertieRef.birthname == bertie.birthname);

    auto& tuppyRef = std::get<Acquaintance>(truculentTrio);
    REQUIRE(tuppyRef.nickname == hildebrand.nickname);
}

/* Tuple */

/* Any */

struct EscapePod {
    EscapePod(int x) : weightKG{x} {}
    int weightKG;
};

TEST_CASE("std::any allows us to std::any_cast into a type") {
    // std::any hagunemnon;
    // hagunemnon.emplace<EscapePod>(600);
    std::any hagunemnon{EscapePod(600)};
    auto pod = std::any_cast<EscapePod>(hagunemnon);
    REQUIRE(pod.weightKG == 600);
    REQUIRE_THROWS_AS(std::any_cast<float>(hagunemnon), std::bad_any_cast);
}

/* Any */

/* Variant */

struct BugblatterBeast {
    BugblatterBeast() : isRavenous{true}, weightKG{20000} {}

    bool isRavenous;
    int weightKG;
};

TEST_CASE("std::variant") {
    std::variant<BugblatterBeast, EscapePod> hagunemnon;
    REQUIRE(hagunemnon.index() == 0);

    hagunemnon.emplace<EscapePod>(800);
    REQUIRE(hagunemnon.index() == 1);

    REQUIRE(std::get<EscapePod>(hagunemnon).weightKG == 800);
    REQUIRE(std::get<1>(hagunemnon).weightKG == 800);
    REQUIRE_THROWS_AS(std::get<0>(hagunemnon), std::bad_variant_access);

    auto lbs = std::visit([](auto& x) { return 2.2 * x.weightKG; }, hagunemnon);
    REQUIRE(lbs == Approx(1760));
}

/* Variant */
