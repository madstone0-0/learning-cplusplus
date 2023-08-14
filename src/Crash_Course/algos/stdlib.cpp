#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <numeric>
#include <string>
#include <vector>

using namespace std;
using Catch::Approx;

/* Non Modifying */

/* Determines whether each element in a
 * sequence meets some user specified criteria
 */
TEST_CASE("all_of") {
    vector<string> words{"Auntie", "Annes", "Alligator"};
    const auto starts_with_a = [](const auto& word) {
        if (word.empty()) return false;
        return word[0] == 'a' || word[0] == 'A';
    };

    REQUIRE(all_of(words.cbegin(), words.cend(), starts_with_a));

    const auto has_length_six = [](const auto& word) { return word.length() == 6; };
    REQUIRE_FALSE(all_of(words.cbegin(), words.cend(), has_length_six));
}

/* Determines whether any element in a
 * sequence meets some user specified criteria
 */
TEST_CASE("any_of") {
    vector<string> words{"Barber", "baby", "bubbles"};
    const auto contains_bar = [](const auto& word) { return word.find("Bar") != string::npos; };
    REQUIRE(any_of(words.cbegin(), words.cend(), contains_bar));

    const auto is_empty = [](const auto& word) { return word.empty(); };
    REQUIRE_FALSE(any_of(words.cbegin(), words.cend(), is_empty));
}

/* Determines whether each element in a
 * sequence does not meet some user specified criteria
 */
TEST_CASE("none_of") {
    vector<string> words{"Camel", "on", "the", "ceiling"};
    const auto is_hump_day = [](const auto& word) { return word == "hump day"; };
    REQUIRE(none_of(words.cbegin(), words.cend(), is_hump_day));

    const auto is_definite_article = [](const auto& word) { return word == "the" || word == "ye"; };
    REQUIRE_FALSE(none_of(words.cbegin(), words.cend(), is_definite_article));
}

/* Applies a function to each element in a
 * sequence
 */
TEST_CASE("for_each") {
    vector<string> words{"David", "Donald", "Doo"};
    size_t number_of_Ds{};

    const auto count_number_of_ds = [&number_of_Ds](const auto& word) {
        if (word.empty()) return;
        if (word[0] == 'D') number_of_Ds++;
    };

    for_each(words.cbegin(), words.cend(), count_number_of_ds);
    REQUIRE(number_of_Ds == 3);
}

/* Applies a function to the first n elements in a
 * sequence
 */
TEST_CASE("for_each_n") {
    vector<string> words{"ear", "egg", "elephant"};
    size_t characters{};
    const auto count_characters = [&characters](const auto& word) { characters += word.size(); };

    for_each_n(words.cbegin(), words.size(), count_characters);
    REQUIRE(14 == characters);
}

/* Find the first element in  a sequence
 * matching  some user defined criteria returning
 * an InputIterator
 */
TEST_CASE("find find_if find_if_not") {
    vector<string> words{"fiffer", "feffer", "feff"};
    const auto find_result = find(words.cbegin(), words.cend(), "feff");
    REQUIRE(*find_result == words.back());

    const auto defends_digital_privacy = [](const auto& word) { return string::npos != word.find("eff"); };

    const auto find_if_result = find_if(words.cbegin(), words.cend(), defends_digital_privacy);

    REQUIRE(*find_if_result == "feffer");

    const auto find_if_not_result = find_if_not(words.cbegin(), words.cend(), defends_digital_privacy);
    REQUIRE(*find_if_not_result == words.front());
}

/* Finds the first repeat in a sequence.
 */
TEST_CASE("adjecent_find") {
    vector<string> words{"Icabod", "is", "itchy"};
    const auto first_letters_match = [](const auto& word1, const auto& word2) {
        if (word1.empty() || word2.empty()) return false;
        return word1.front() == word2.front();
    };

    const auto adjacent_find_result = adjacent_find(words.cbegin(), words.cend(), first_letters_match);
    REQUIRE(*adjacent_find_result == words[1]);
}

/* Counts the elements in a sequence
 * matching some user defined criteria
 */
TEST_CASE("count count_if") {
    vector<string> words{"jelly", "jar", "and", "jam"};
    const auto n_ands = count(words.cbegin(), words.cend(), "and");
    REQUIRE(n_ands == 1);

    const auto contains_a = [](const auto& word) { return word.find('a') != string::npos; };

    const auto count_if_res = count_if(words.cbegin(), words.cend(), contains_a);
    REQUIRE(count_if_res == 3);
}

/* Finds the first mismatch in two sequences.
 */
TEST_CASE("mismatch") {
    vector<string> words1{"Kitten", "Kangaroo", "Kick"};
    vector<string> words2{"Kitten", "bandicoot", "roundhouse"};

    const auto mismatch_res1 = mismatch(words1.cbegin(), words1.cend(), words2.cbegin());
    REQUIRE(*mismatch_res1.first == "Kangaroo");
    REQUIRE(*mismatch_res1.second == "bandicoot");

    const auto second_letter_matches = [](const auto& word1, const auto& word2) {
        if (word1.size() < 2) return false;
        if (word2.size() < 2) return false;

        return word1[1] == word2[1];
    };

    const auto mismatch_res2 = mismatch(words1.cbegin(), words1.cend(), words2.cbegin(), second_letter_matches);
    REQUIRE(*mismatch_res2.first == "Kick");
    REQUIRE(*mismatch_res2.second == "roundhouse");
}

/* Determines whether two sequences are equal
 */
TEST_CASE("equal") {
    vector<string> words1{"Lazy", "lion", "licks"};
    vector<string> words2{"Lazy", "lion", "kicks"};

    const auto equal_result1 = equal(words1.cbegin(), words1.cend(), words2.cbegin());
    REQUIRE_FALSE(equal_result1);

    words2[2] = words1[2];
    const auto equal_result2 = equal(words1.cbegin(), words1.cend(), words2.cbegin());
    REQUIRE(equal_result2);
}

/* Determines whether two sequences are permutations
 * i.e. contain the same elements but in differing order
 */
TEST_CASE("is_permutation") {
    vector<string> words1{"moonlight", "mighty", "nice"};
    vector<string> words2{"nice", "moonlight", "mighty"};
    const auto result = is_permutation(words1.cbegin(), words1.cend(), words2.cbegin());
    REQUIRE(result);
}

/* Locates a subsequence in a sequence
 */
TEST_CASE("search") {
    vector<string> words1{"Nine", "new", "neckties", "and", "a", "nightshirt"};
    vector<string> words2{"and", "a", "nightshirt"};
    const auto search_res_1 = search(words1.cbegin(), words1.cend(), words2.cbegin(), words2.cend());

    REQUIRE(*search_res_1 == "and");

    vector<string> words3{"and", "a", "nightpant"};
    const auto search_res_2 = search(words1.cbegin(), words1.cend(), words3.cbegin(), words3.cend());
    REQUIRE(search_res_2 == words1.cend());
}

/* Locates  a subsequence containing identical
 * consecutive values
 */
TEST_CASE("search_n") {
    vector<string> words{"an", "orange", "owl", "owl", "owl", "today"};
    const auto res = search_n(words.cbegin(), words.cend(), 3, "owl");

    REQUIRE(res == words.cbegin() + 2);
}

/* Non Modifying */

/* Modifying */

/*  Copies one sequence into another
 */
TEST_CASE("copy") {
    vector<string> words1{"and", "prosper"};
    vector<string> words2{"Live", "long"};
    copy(words1.cbegin(), words1.cend(), back_inserter(words2));
    REQUIRE(words2 == vector<string>{"Live", "long", "and", "prosper"});
}

struct MoveDetector {
    MoveDetector() : owner{true} {}
    MoveDetector(const MoveDetector&) = delete;
    MoveDetector& operator=(const MoveDetector&) = delete;
    MoveDetector(MoveDetector&& other) = delete;
    MoveDetector& operator=(MoveDetector&& other) {
        other.owner = false;
        owner = true;
        return *this;
    }

    bool owner;
};

/* Moves one sequence into another
 */
TEST_CASE("move") {
    vector<MoveDetector> detectors1(2);
    vector<MoveDetector> detectors2(2);

    move(detectors1.begin(), detectors1.end(), detectors2.begin());

    REQUIRE_FALSE(detectors1[0].owner);
    REQUIRE_FALSE(detectors1[1].owner);
    REQUIRE(detectors2[0].owner);
    REQUIRE(detectors2[1].owner);
}

/* Swaps elements from one sequence into
 * another
 */
TEST_CASE("swap_ranges") {
    vector<string> words1{"The", "king", "is", "dead"};
    vector<string> words2{"Long", "live", "the", "king"};
    swap_ranges(words1.begin(), words1.end(), words2.begin());
    REQUIRE(words1 == vector<string>{"Long", "live", "the", "king"});
    REQUIRE(words2 == vector<string>{"The", "king", "is", "dead"});
}

/* Modifies the elements of one sequence
 * and writes them into another
 */
#include <boost/algorithm/string/case_conv.hpp>
TEST_CASE("transform") {
    vector<string> words1{"farewell", "hello", "farewell", "hello"};
    vector<string> res1;

    const auto upper = [](string word) {
        boost::to_upper(word);
        return word;
    };
    transform(words1.begin(), words1.end(), back_inserter(res1), upper);
    REQUIRE(res1 == vector<string>{"FAREWELL", "HELLO", "FAREWELL", "HELLO"});

    vector<string> words2{"light", "human", "bro", "quantum"};
    vector<string> words3{"radar", "robot", "pony", "bit"};
    vector<string> res2;

    auto portmantize = [](const auto& word1, const auto& word2) {
        const auto word1_letters = min(size_t{2}, word1.size());
        string res{word1.begin(), word1.begin() + word1_letters};

        const auto word2_letters = min(size_t{3}, word2.size());
        res.insert(res.end(), word2.end() - word2_letters, word2.end());
        return res;
    };

    transform(words2.begin(), words2.end(), words3.begin(), back_inserter(res2), portmantize);
    REQUIRE(res2 == vector<string>{"lidar", "hubot", "brony", "qubit"});
}

/* Replaces certain elements in a sequence
 * with some new element
 */
#include <string_view>
TEST_CASE("replace") {
    using namespace string_literals;

    vector<string> words1{"There", "is", "no", "try"};
    replace(words1.begin(), words1.end(), "try"sv, "spoon"sv);

    REQUIRE(words1 == vector<string>{"There", "is", "no", "spoon"});

    const vector<string> words2{"There", "is", "no", "spoon"};
    vector<string> words3{"There", "is", "no", "spoon"};
    auto has_two_oos = [](string_view x) { return count(x.begin(), x.end(), 'o') == 2; };

    replace_copy_if(words2.begin(), words2.end(), words3.begin(), has_two_oos, "try"sv);
    REQUIRE(words3 == vector<string>{"There", "is", "no", "try"});
}

/* Fills a sequence with some value
 */
TEST_CASE("fill") {
    vector<string> answer1(6);
    fill(answer1.begin(), answer1.end(), "police");
    REQUIRE(answer1 == vector<string>{"police", "police", "police", "police", "police", "police"});

    vector<string> answer2;
    fill_n(back_inserter(answer2), 6, "police");
    REQUIRE(answer2 == vector<string>{"police", "police", "police", "police", "police", "police"});
}

/* Fills a sequence by invoking a function
 * object
 */
TEST_CASE("generate") {
    auto i{1};
    auto pow_of_2 = [&i]() {
        const auto tmp = i;
        i *= 2;
        return tmp;
    };

    vector<int> series1(6);
    generate(series1.begin(), series1.end(), pow_of_2);
    REQUIRE(series1 == vector<int>{1, 2, 4, 8, 16, 32});

    vector<int> series2;
    generate_n(back_inserter(series2), 6, pow_of_2);
    REQUIRE(series2 == vector<int>{64, 128, 256, 512, 1024, 2048});
}

/* Removes repeat elements in a sequence
 */
TEST_CASE("unique") {
    string without_walls{"Walless"};
    const auto new_end = unique(without_walls.begin(), without_walls.end());
    without_walls.erase(new_end, without_walls.end());

    REQUIRE(without_walls == "Wales");
}

/* Modifying */

/* Sorting and Related */

/* Sorts a sequence unstably
 */
TEST_CASE("sort") {
    string goat_grass{"spoilage"};
    sort(goat_grass.begin(), goat_grass.end());
    REQUIRE(goat_grass == "aegilops");
}

enum CharCategory { Ascender, Normal, Descender };

CharCategory categorize(char x) {
    switch (x) {
        case 'g':
        case 'j':
        case 'p':
        case 'q':
        case 'y':
            return CharCategory::Descender;
        case 'b':
        case 'd':
        case 'f':
        case 'h':
        case 'k':
        case 'l':
        case 't':
            return CharCategory::Ascender;
    }
    return CharCategory::Normal;
}

bool ascension_compare(char x, char y) { return categorize(x) < categorize(y); }

/* Sorts a sequence stably
 */
TEST_CASE("stable_sort") {
    string word{"outgrin"};
    stable_sort(word.begin(), word.end(), ascension_compare);

    REQUIRE(word == "touring");
}

/* Sorts a sequence into two groups.
 * Sorts the first half of the specified sequence, where half is
 * user defined and leaves the remaining elements unsorted
 */
TEST_CASE("partial_sort") {
    string word1{"nectarous"};
    partial_sort(word1.begin(), word1.begin() + 4, word1.end());
    REQUIRE(word1 == "acentrous");

    string word2{"pretanning"};
    partial_sort(word2.begin(), word2.begin() + 3, word2.end(), ascension_compare);
    REQUIRE(word2 == "trepanning");
}

/* Determines whether a sequence is sorted
 */
TEST_CASE("is_sorted") {
    string word1{"billowy"};
    REQUIRE(is_sorted(word1.begin(), word1.end()));

    string word2{"floppy"};
    REQUIRE(is_sorted(word2.begin(), word2.end(), ascension_compare));
}

/* Binary Search */

/* Finds the first position in a sorted sequence where
 * a value could be inserted without changing the ordering of the
 * sequence
 */
TEST_CASE("lower_bound") {
    vector<int> numbers{2, 4, 5, 6, 6, 9};
    const auto res = lower_bound(numbers.begin(), numbers.end(), 5);
    REQUIRE(res == numbers.begin() + 2);
}

/* Finds the last position in a sorted sequence where
 * a value could be inserted without changing the ordering of the
 * sequence
 */
TEST_CASE("upper_bound") {
    vector<int> numbers{2, 4, 5, 6, 6, 9};
    const auto res = upper_bound(numbers.begin(), numbers.end(), 5);
    REQUIRE(res == numbers.begin() + 3);
}

/* Finds a particular element in a
 * sorted sequence
 */
TEST_CASE("binary_search") {
    vector<int> numbers{2, 4, 5, 6, 6, 9};
    REQUIRE(binary_search(numbers.begin(), numbers.end(), 6));
    REQUIRE_FALSE(binary_search(numbers.begin(), numbers.end(), 7));
}

/* Binary Search */

/* Partitioning */

/* Determines whether a sequence is
 * partitioned
 */
TEST_CASE("is_partitioned") {
    auto is_odd = [](auto x) { return x % 2 == 1; };

    vector<int> numbers1{9, 5, 9, 6, 4, 2};
    REQUIRE(is_partitioned(numbers1.begin(), numbers1.end(), is_odd));

    vector<int> numbers2{9, 4, 9, 6, 4, 2};
    REQUIRE_FALSE(is_partitioned(numbers2.begin(), numbers2.end(), is_odd));
}

/* Partitions a sequence according to
 * some user defined criteria
 */
TEST_CASE("partition") {
    auto is_odd = [](auto x) { return x % 2 == 1; };

    vector<int> numbers{1, 2, 3, 4, 5};
    const auto partition_point = partition(numbers.begin(), numbers.end(), is_odd);

    REQUIRE(is_partitioned(numbers.begin(), numbers.end(), is_odd));
    REQUIRE(partition_point == numbers.begin() + 3);
}

/* Partitions a sequence according to
 * some user defined criteria dividing
 * each part into the provided sequences
 */
TEST_CASE("partition_copy") {
    auto is_odd = [](auto x) { return x % 2 == 1; };

    vector<int> numbers{1, 2, 3, 4, 5}, odds, evens;
    partition_copy(numbers.begin(), numbers.end(), back_inserter(odds), back_inserter(evens), is_odd);

    REQUIRE(all_of(odds.begin(), odds.end(), is_odd));
    REQUIRE(none_of(evens.begin(), evens.end(), is_odd));
}

/* Partitioning */

/* Merges two sorted
 * sequences
 */
TEST_CASE("merge") {
    vector<int> numbers1{1, 4, 5}, numbers2{2, 3, 3, 6}, result;
    merge(numbers1.begin(), numbers1.end(), numbers2.begin(), numbers2.end(), back_inserter(result));

    REQUIRE(result == vector<int>{1, 2, 3, 3, 4, 5, 6});
}

/* Extreme Value */

/* Determines a sequences extrema
 */
TEST_CASE("max min minmax") {
    auto length_compare = [](const auto& x1, const auto& x2) { return x1.length() < x2.length(); };

    string undisc = "undiscriminativeness", vermin = "vermin";
    REQUIRE(min(undisc, vermin, length_compare) == "vermin");

    string maxim = "maxim", ultra = "ultramaximal";
    REQUIRE(max(maxim, ultra, length_compare) == "ultramaximal");

    string mini = "minimaxes", maxi = "maximin";
    const auto res = minmax(mini, maxi, length_compare);
    REQUIRE(res.first == maxi);
    REQUIRE(res.second == mini);
}

/* Determines a sequences extrema
 */
TEST_CASE("max_element min_element") {
    auto length_compare = [](const auto& x1, const auto& x2) { return x1.length() < x2.length(); };

    vector<string> words{"civic", "deed", "kayak", "malayalam"};

    REQUIRE(*min_element(words.begin(), words.end(), length_compare) == "deed");
    REQUIRE(*max_element(words.begin(), words.end(), length_compare) == "malayalam");

    const auto res = minmax_element(words.begin(), words.end(), length_compare);
    REQUIRE(*res.first == "deed");
    REQUIRE(*res.second == "malayalam");
}

/* Bounds a value
 */
TEST_CASE("clamp") {
    REQUIRE(clamp(9000, 0, 100) == 100);
    REQUIRE(clamp(-123, 0, 100) == 0);
    REQUIRE(clamp(3.14, 0., 100.) == Approx(3.14));
}

/* Extreme Value */

/* Numbers */

/* Basically python's range()
 */
TEST_CASE("iota") {
    array<int, 3> easyAs;
    iota(easyAs.begin(), easyAs.end(), 1);
    REQUIRE(easyAs == array<int, 3>{1, 2, 3});
}

/* Folds a sequence in order
 */
TEST_CASE("accumulate") {
    vector<int> nums{1, 2, 3};
    const auto res1 = accumulate(nums.begin(), nums.end(), -1);
    REQUIRE(res1 == 5);

    const auto res2 = accumulate(nums.begin(), nums.end(), 2, multiplies());

    REQUIRE(res2 == 12);
}

/* Folds a sequence
 */
TEST_CASE("reduce") {
    vector<int> nums{1, 2, 3};
    const auto res1 = reduce(nums.begin(), nums.end(), -1);
    REQUIRE(res1 == 5);

    const auto res2 = reduce(nums.begin(), nums.end(), 2, multiplies());
    REQUIRE(res2 == 12);
}

/* Calculates the dot product of two sequences
 */
TEST_CASE("inner_product") {
    vector<int> nums1{1, 2, 3, 4, 5};
    vector<int> nums2{1, 0, -1, 0, 1};
    const auto result = inner_product(nums1.begin(), nums1.end(), nums2.begin(), 10);
    REQUIRE(result == 13);
}

/* Numbers */

/* Sorting and Related */
