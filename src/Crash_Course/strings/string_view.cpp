
#include <catch2/catch_all.hpp>
#include <string>
#include <string_view>

/* Constructing */

using std::string_view, std::string;
TEST_CASE("std::string_view supports") {
    SECTION("default construction") {
        string_view view{};
        REQUIRE(view.data() == nullptr);
        REQUIRE(view.size() == 0);
        REQUIRE(view.empty());
    }

    SECTION("constructing from string") {
        string word{"sacrosanct"};
        string_view view(word);
        REQUIRE(view == "sacrosanct");
    }

    SECTION("construction from c string") {
        auto word = "viewership";
        string_view view(word);
        REQUIRE(view == "viewership");
    }

    SECTION("constructing from c string and length") {
        auto word = "viewership";
        string_view view(word, 4);
        REQUIRE(view == "view");
    }
}

/* Constructing */

/* Ops */

TEST_CASE("std::string_view is modifiable with") {
    string_view view("previewing");
    SECTION("remove_prefix") {
        view.remove_prefix(3);
        REQUIRE(view == "viewing");
    }

    SECTION("remove_suffix") {
        view.remove_suffix(3);
        REQUIRE(view == "preview");
    }
}

/* Ops */
