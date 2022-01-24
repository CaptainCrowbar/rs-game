#include "rs-game/text-gen.hpp"
#include "rs-format/string.hpp"
#include "rs-sci/random.hpp"
#include "rs-unit-test.hpp"
#include <map>
#include <string>

using namespace RS::Format;
using namespace RS::Game;
using namespace RS::Sci;

void test_rs_game_text_generation_null() {

    TextGenerator t;
    StdRng rng(42);
    std::string s;

    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_EQUAL(s, "");
    }

}

void test_rs_game_text_generation_constant() {

    TextGenerator t;
    StdRng rng(42);
    std::string s;

    TRY(t = "hello");

    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_EQUAL(s, "hello");
    }

    TRY(t = U'α');

    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_EQUAL(s, "α");
    }

}

void test_rs_game_text_generation_number() {

    TextGenerator t;
    StdRng rng(42);
    std::string s;

    TRY(t = number(100, 299));

    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_MATCH(s, "^[12]\\d\\d$");
    }

}

void test_rs_game_text_generation_select() {

    TextGenerator t;
    StdRng rng(42);
    std::string s;
    std::map<std::string, int> census;

    TRY(t = choose({"abc", "def", "ghi"}));

    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_MATCH(s, "^(abc|def|ghi)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 3u);
    TEST_NEAR(census["abc"], 333, 50);
    TEST_NEAR(census["def"], 333, 50);
    TEST_NEAR(census["ghi"], 333, 50);

    TextList list = {"rst", "uvw", "xyz"};
    TRY(t = choose(list));
    census.clear();

    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_MATCH(s, "^(rst|uvw|xyz)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 3u);
    TEST_NEAR(census["rst"], 333, 50);
    TEST_NEAR(census["uvw"], 333, 50);
    TEST_NEAR(census["xyz"], 333, 50);

    TRY(t = str("abc") | "def" | "ghi" | "jkl");
    census.clear();

    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_MATCH(s, "^(abc|def|ghi|jkl)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 4u);
    TEST_NEAR(census["abc"], 250, 50);
    TEST_NEAR(census["def"], 250, 50);
    TEST_NEAR(census["ghi"], 250, 50);
    TEST_NEAR(census["jkl"], 250, 50);

    TRY(t = choose("αβγδε"));
    census.clear();

    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_MATCH(s, "^(α|β|γ|δ|ε)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 5u);
    TEST_NEAR(census["α"], 200, 50);
    TEST_NEAR(census["β"], 200, 50);
    TEST_NEAR(census["γ"], 200, 50);
    TEST_NEAR(census["δ"], 200, 50);
    TEST_NEAR(census["ε"], 200, 50);

}

void test_rs_game_text_generation_weighted() {

    TextGenerator t;
    StdRng rng(42);
    std::string s;
    std::map<std::string, int> census;

    TRY(t = choose({
        { "abc", 1 },
        { "def", 2 },
        { "ghi", 3 },
        { "jkl", 4 },
    }));


    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_MATCH(s, "^(abc|def|ghi|jkl)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 4u);
    TEST_NEAR(census["abc"], 100, 50);
    TEST_NEAR(census["def"], 200, 50);
    TEST_NEAR(census["ghi"], 300, 50);
    TEST_NEAR(census["jkl"], 400, 50);

    TextWeights weights = {
        { "opq", 1 },
        { "rst", 2 },
        { "uvw", 3 },
        { "xyz", 4 },
    };

    TRY(t = choose(weights));
    census.clear();

    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_MATCH(s, "^(opq|rst|uvw|xyz)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 4u);
    TEST_NEAR(census["opq"], 100, 50);
    TEST_NEAR(census["rst"], 200, 50);
    TEST_NEAR(census["uvw"], 300, 50);
    TEST_NEAR(census["xyz"], 400, 50);

}

void test_rs_game_text_generation_sequence() {

    TextGenerator t, u, v;
    StdRng rng(42);
    std::string s;

    TRY(t = choose({"abc", "def", "ghi"}));
    TRY(u = choose({"123", "456", "789"}));
    TRY(v = t + u);

    for (int i = 0; i < 1000; ++i) {
        TRY(s = v(rng));
        TEST_MATCH(s, "^(abc|def|ghi)(123|456|789)$");
    }

}

void test_rs_game_text_generation_optional() {

    TextGenerator t, u;
    StdRng rng(42);
    std::string s;
    std::map<std::string, int> census;

    TRY(u = "abc");
    TRY(t = u % 0.75);

    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_MATCH(s, "^(abc)?$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 2u);
    TEST_NEAR(census["abc"], 750, 50);
    TEST_NEAR(census[""], 250, 50);

}

void test_rs_game_text_generation_repeat() {

    TextGenerator t, u;
    StdRng rng(42);
    std::string s;
    std::map<std::string, int> census;

    TRY(u = "abc");
    TRY(t = u * 3);

    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_EQUAL(s, "abcabcabc");
    }

    TRY(u = "xyz");
    TRY(t = u * 1 * 5);

    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_MATCH(s, "^(xyz){1,5}$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 5u);
    TEST_NEAR(census["xyz"], 200, 50);
    TEST_NEAR(census["xyzxyz"], 200, 50);
    TEST_NEAR(census["xyzxyzxyz"], 200, 50);
    TEST_NEAR(census["xyzxyzxyzxyz"], 200, 50);
    TEST_NEAR(census["xyzxyzxyzxyzxyz"], 200, 50);

}

void test_rs_game_text_generation_transform() {

    TextGenerator t, u;
    StdRng rng(42);
    std::string s;
    std::map<std::string, int> census;

    TRY(u = str("abc") | "def" | "ghi");
    TRY(t = u >> ascii_uppercase);

    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_MATCH(s, "^(ABC|DEF|GHI)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 3u);
    TEST_NEAR(census["ABC"], 333, 50);
    TEST_NEAR(census["DEF"], 333, 50);
    TEST_NEAR(census["GHI"], 333, 50);

    TRY(t = u >> [] (auto& s) { return "(" + s + ")"; });
    census.clear();

    for (int i = 0; i < 1000; ++i) {
        TRY(s = t(rng));
        TEST_MATCH(s, "^\\((abc|def|ghi)\\)$");
        ++census[s];
    }

    TEST_EQUAL(census.size(), 3u);
    TEST_NEAR(census["(abc)"], 333, 50);
    TEST_NEAR(census["(def)"], 333, 50);
    TEST_NEAR(census["(ghi)"], 333, 50);

}
