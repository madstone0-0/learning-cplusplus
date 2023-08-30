
#include <catch2/catch_all.hpp>
#include <string>

/* Constructing */

using std::string;
using namespace Catch;
TEST_CASE("std::string supports constructing") {
    SECTION("Empty strings") {
        string cheese;
        REQUIRE(cheese.empty());
    }

    SECTION("repeated characters") {
        string roadsideAssistance(3, 'A');
        REQUIRE(roadsideAssistance == "AAA");
    }
}

TEST_CASE("std::string supports constructing substrings") {
    auto word = "fimbulvinter";
    REQUIRE(string(word) == "fimbulvinter");
    REQUIRE(string(word, 6) == "fimbul");
}

TEST_CASE("std::string supports ") {
    string word = "jourmungandr";
    SECTION("Copy construction") { REQUIRE(string(word) == "jourmungandr"); }

    SECTION("Move constructing") {
        REQUIRE(string(std::move(word)) == "jourmungandr");
        REQUIRE(word.empty());
    }

    SECTION("Substr") {
        REQUIRE(string(word, 0, 7) == "jourmun");
        REQUIRE(string(word, 7) == "gandr");
    }
}

TEST_CASE("std::string supports constructing a string with") {
    SECTION("std::string(char*) stops at embedded nulls") {
        string str("idioglossia\0ellohay!");
        REQUIRE(str.length() == 11);
    }

    SECTION("operator\"\"s incorporates embedded nulls") {
        using namespace std::string_literals;

        auto strLit = "idioglossia\0ellohay!"s;
        REQUIRE(strLit.length() == 20);
    }
}

/* Constructing */

/* Iterators */

TEST_CASE("string's c_str method makes null terminated strings") {
    string word{"Horripliation"};
    auto cstr = word.c_str();
    REQUIRE(cstr[0] == 'H');
    REQUIRE(cstr[1] == 'o');
    REQUIRE(cstr[11] == 'o');
    REQUIRE(cstr[12] == 'n');
    REQUIRE(cstr[13] == '\0');
}

/* Iterators */

/* Comparisions */

TEST_CASE("std::string supports comparison with ") {
    using namespace std::string_literals;
    string word{"allusion"};
    SECTION("operator== and !=") {
        REQUIRE(word == "allusion");
        REQUIRE(word == "allusion"s);
        REQUIRE(word != "Allusion"s);
        REQUIRE(word != "illusion"s);
        REQUIRE_FALSE(word == "illusion"s);
    }

    SECTION("operator<") {
        REQUIRE(word < "illusion");
        REQUIRE(word < "illusion"s);
        REQUIRE(word > "Illusion"s);
    }
}

/* Comparisons */

/* Manipulation */

// Adding
TEST_CASE("std::string supports appending with") {
    string word{"butt"};
    SECTION("push_back") {
        word.push_back('e');
        REQUIRE(word == "butte");
    }

    SECTION("operator+=") {
        word += "erfinger";
        REQUIRE(word == "butterfinger");
    }

    SECTION("append char") {
        word.append(1, 's');
        REQUIRE(word == "butts");
    }

    SECTION("append char*") {
        word.append("stockings", 5);
        REQUIRE(word == "buttstock");
    }

    SECTION("append half opened range") {
        string other{"onomatopoeia"};
        word.append(other.begin(), other.begin() + 2);
        REQUIRE(word == "button");
    }
}

// Removal
TEST_CASE("std::string supports removal with") {
    string word{"therein"};

    SECTION("pop_back") {
        word.pop_back();
        word.pop_back();
        REQUIRE(word == "there");
    }

    SECTION("clear") {
        word.clear();
        REQUIRE(word.empty());
    }

    SECTION("erase using half opened range") {
        word.erase(word.begin(), word.begin() + 3);
        REQUIRE(word == "rein");
    }

    SECTION("erase using an index and length") {
        word.erase(5, 2);
        REQUIRE(word == "there");
    }

    SECTION("erase using an iterator") {
        word.erase(word.begin() + 5);
        REQUIRE(word == "theren");
    }
}

// Replacing
TEST_CASE("std::string supports replacing with") {
    string word{"substitution"};
    SECTION("a range and a char") {
        word.replace(word.begin() + 9, word.end(), "e");
        REQUIRE(word == "substitute");
    }

    SECTION("two ranges") {
        string other{"innuendo"};
        word.replace(word.begin(), word.begin() + 3, other.begin(), other.begin() + 2);
        REQUIRE(word == "institution");
    }

    SECTION("an index/length and a string") {
        string other{"vers"};
        word.replace(3, 6, other);
        REQUIRE(word == "subversion");
    }
}

// Resize
TEST_CASE("std::string resize") {
    string word{"shamp"};
    SECTION("can remove elements") {
        word.resize(4);
        REQUIRE(word == "sham");
    }

    SECTION("can add elements") {
        word.resize(7, 'o');
        REQUIRE(word == "shampoo");
    }
}

// Substr
TEST_CASE("std::string substr with") {
    string word{"hobbits"};

    SECTION("no args copies") { REQUIRE(word.substr() == "hobbits"); }

    SECTION("position takes the remainder") { REQUIRE(word.substr(3) == "bits"); }

    SECTION("position/index takes a substr") { REQUIRE(word.substr(3, 3) == "bit"); }
}

/* Manipulation */

/* Search */

TEST_CASE("std::string find") {
    using namespace std::string_literals;
    string word{"pizzazz"};
    SECTION("locate substrings from strings") { REQUIRE(word.find("zz"s) == 2); }
    SECTION("accepts pos arg") { REQUIRE(word.find("zz"s, 3) == 5); }
    SECTION("locates substrings from char") { REQUIRE(word.find("zaz") == 3); }
    SECTION("returns npos when not found") { REQUIRE(word.find("x") == string::npos); }
}

TEST_CASE("std::string rfind") {
    using namespace std::string_literals;
    string word{"pizzazz"};

    SECTION("locates substrings from strings") { REQUIRE(word.rfind("zz"s, 3) == 2); }
    SECTION("locates substrings from char") { REQUIRE(word.rfind("zaz") == 3); }
    SECTION("returns npos when not found") { REQUIRE(word.rfind("x") == string::npos); }
}

// find_*_of
TEST_CASE("std::string find_first_of") {
    using namespace std::string_literals;
    string sentence{"I am a Zizzer-Zazzer-Zuzz as you can plainly see."};
    SECTION("locates characters within another string") { REQUIRE(sentence.find_first_of("Zz"s) == 7); }

    SECTION("accepts a position argument") { REQUIRE(sentence.find_first_of("Zz"s, 11) == 14); }

    SECTION("returns npos when not found") { REQUIRE(sentence.find_first_of("Xx"s) == string::npos); }
}

TEST_CASE("std::string") {
    using namespace std::literals::string_literals;
    std::string sentence("I am a Zizzer-Zazzer-Zuzz as you can plainly see.");
    SECTION("find_last_of finds last element within another string") {
        REQUIRE(sentence.find_last_of("Zz"s) == 24);  // Zuz(z)
    }

    SECTION("find_first_not_of finds first element not within another string") {
        REQUIRE(sentence.find_first_not_of(" -IZaeimrz"s) == 22);  // Z(u)zz
    }

    SECTION("find_last_not_of finds last element not within another string") {
        REQUIRE(sentence.find_last_not_of(" .es"s) == 43);  // plainl(y)
    }
}

/* Search */

/* Conversion */

TEST_CASE("STL string conversion function") {
    using namespace std::string_literals;
    SECTION("to_string") { REQUIRE("8975309"s == std::to_string(8975309)); }
    SECTION("to_wstring") { REQUIRE(L"109951.162776"s == std::to_wstring(109951.162776)); }

    SECTION("stoi") { REQUIRE(std::stoi("8675309"s) == 8675309); }

    SECTION("stoul with all valid characters") {
        size_t last_character{};
        const auto result = std::stoul("0xD3C34C3D"s, &last_character, 16);
        REQUIRE(result == 0xD3C34C3D);
        REQUIRE(last_character == 10);
    }

    SECTION("stoul") {
        size_t last_character{};
        const auto result = std::stoul("42six"s, &last_character);
        REQUIRE(result == 42);
        REQUIRE(last_character == 2);
    }

    SECTION("stod") { REQUIRE(std::stod("2.7182818"s) == Approx(2.7182818)); }
}

/* Conversion */
