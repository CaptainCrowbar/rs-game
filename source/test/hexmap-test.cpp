#include "rs-game/hexmap.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-unit-test.hpp"
#include <algorithm>
#include <functional>
#include <iostream>

using namespace RS::Game;
using namespace RS::Graphics::Core;
using namespace RS::UnitTest;

namespace {

    static constexpr auto cmp_hex = std::less<Hexmap::hex>();

}

void test_rs_game_hexmap_coordinate_operations() {

    using namespace RS::Game::Detail;

    TEST(! Hexmap::is_valid({-1,-1}));
    TEST(! Hexmap::is_valid({-1,0}));
    TEST(! Hexmap::is_valid({-1,1}));
    TEST(! Hexmap::is_valid({0,-1}));
    TEST(Hexmap::is_valid({0,0}));
    TEST(! Hexmap::is_valid({0,1}));
    TEST(! Hexmap::is_valid({1,-1}));
    TEST(Hexmap::is_valid({1,0}));
    TEST(Hexmap::is_valid({1,1}));
    TEST(Hexmap::is_valid({1,2}));
    TEST(Hexmap::is_valid({1,3}));
    TEST(Hexmap::is_valid({1,4}));
    TEST(Hexmap::is_valid({1,5}));
    TEST(! Hexmap::is_valid({1,6}));
    TEST(! Hexmap::is_valid({2,-1}));
    TEST(Hexmap::is_valid({2,0}));
    TEST(Hexmap::is_valid({2,1}));
    TEST(Hexmap::is_valid({2,2}));
    TEST(Hexmap::is_valid({2,3}));
    TEST(Hexmap::is_valid({2,4}));
    TEST(Hexmap::is_valid({2,5}));
    TEST(Hexmap::is_valid({2,6}));
    TEST(Hexmap::is_valid({2,7}));
    TEST(Hexmap::is_valid({2,8}));
    TEST(Hexmap::is_valid({2,9}));
    TEST(Hexmap::is_valid({2,10}));
    TEST(Hexmap::is_valid({2,11}));
    TEST(! Hexmap::is_valid({2,12}));

    // |                3,0                |
    // |           3,17      3,1           |
    // |      3,16      2,0       3,2      |
    // | 3,15      2,11      2,1       3,3 |
    // |      2,10      1,0       2,2      |
    // | 3,14      1,5       1,1       3,4 |
    // |      2,9       0,0       2,3      |
    // | 3,13      1,4       1,2       3,5 |
    // |      2.8       1,3       2,4      |
    // | 3,12      2,7       2,5       3,6 |
    // |      3,11      2,6       3,7      |
    // |           3,10      3,8           |
    // |                3,9                |

    TEST_EQUAL(rs_to_xy({0,0}), Int2(0,0));     TEST_EQUAL(xy_to_rs({0,0}), Hexmap::hex(0,0));
    TEST_EQUAL(rs_to_xy({1,0}), Int2(0,2));     TEST_EQUAL(xy_to_rs({0,2}), Hexmap::hex(1,0));
    TEST_EQUAL(rs_to_xy({1,1}), Int2(2,1));     TEST_EQUAL(xy_to_rs({2,1}), Hexmap::hex(1,1));
    TEST_EQUAL(rs_to_xy({1,2}), Int2(2,-1));    TEST_EQUAL(xy_to_rs({2,-1}), Hexmap::hex(1,2));
    TEST_EQUAL(rs_to_xy({1,3}), Int2(0,-2));    TEST_EQUAL(xy_to_rs({0,-2}), Hexmap::hex(1,3));
    TEST_EQUAL(rs_to_xy({1,4}), Int2(-2,-1));   TEST_EQUAL(xy_to_rs({-2,-1}), Hexmap::hex(1,4));
    TEST_EQUAL(rs_to_xy({1,5}), Int2(-2,1));    TEST_EQUAL(xy_to_rs({-2,1}), Hexmap::hex(1,5));
    TEST_EQUAL(rs_to_xy({2,0}), Int2(0,4));     TEST_EQUAL(xy_to_rs({0,4}), Hexmap::hex(2,0));
    TEST_EQUAL(rs_to_xy({2,1}), Int2(2,3));     TEST_EQUAL(xy_to_rs({2,3}), Hexmap::hex(2,1));
    TEST_EQUAL(rs_to_xy({2,2}), Int2(4,2));     TEST_EQUAL(xy_to_rs({4,2}), Hexmap::hex(2,2));
    TEST_EQUAL(rs_to_xy({2,3}), Int2(4,0));     TEST_EQUAL(xy_to_rs({4,0}), Hexmap::hex(2,3));
    TEST_EQUAL(rs_to_xy({2,4}), Int2(4,-2));    TEST_EQUAL(xy_to_rs({4,-2}), Hexmap::hex(2,4));
    TEST_EQUAL(rs_to_xy({2,5}), Int2(2,-3));    TEST_EQUAL(xy_to_rs({2,-3}), Hexmap::hex(2,5));
    TEST_EQUAL(rs_to_xy({2,6}), Int2(0,-4));    TEST_EQUAL(xy_to_rs({0,-4}), Hexmap::hex(2,6));
    TEST_EQUAL(rs_to_xy({2,7}), Int2(-2,-3));   TEST_EQUAL(xy_to_rs({-2,-3}), Hexmap::hex(2,7));
    TEST_EQUAL(rs_to_xy({2,8}), Int2(-4,-2));   TEST_EQUAL(xy_to_rs({-4,-2}), Hexmap::hex(2,8));
    TEST_EQUAL(rs_to_xy({2,9}), Int2(-4,0));    TEST_EQUAL(xy_to_rs({-4,0}), Hexmap::hex(2,9));
    TEST_EQUAL(rs_to_xy({2,10}), Int2(-4,2));   TEST_EQUAL(xy_to_rs({-4,2}), Hexmap::hex(2,10));
    TEST_EQUAL(rs_to_xy({2,11}), Int2(-2,3));   TEST_EQUAL(xy_to_rs({-2,3}), Hexmap::hex(2,11));
    TEST_EQUAL(rs_to_xy({3,0}), Int2(0,6));     TEST_EQUAL(xy_to_rs({0,6}), Hexmap::hex(3,0));
    TEST_EQUAL(rs_to_xy({3,1}), Int2(2,5));     TEST_EQUAL(xy_to_rs({2,5}), Hexmap::hex(3,1));
    TEST_EQUAL(rs_to_xy({3,2}), Int2(4,4));     TEST_EQUAL(xy_to_rs({4,4}), Hexmap::hex(3,2));
    TEST_EQUAL(rs_to_xy({3,3}), Int2(6,3));     TEST_EQUAL(xy_to_rs({6,3}), Hexmap::hex(3,3));
    TEST_EQUAL(rs_to_xy({3,4}), Int2(6,1));     TEST_EQUAL(xy_to_rs({6,1}), Hexmap::hex(3,4));
    TEST_EQUAL(rs_to_xy({3,5}), Int2(6,-1));    TEST_EQUAL(xy_to_rs({6,-1}), Hexmap::hex(3,5));
    TEST_EQUAL(rs_to_xy({3,6}), Int2(6,-3));    TEST_EQUAL(xy_to_rs({6,-3}), Hexmap::hex(3,6));
    TEST_EQUAL(rs_to_xy({3,7}), Int2(4,-4));    TEST_EQUAL(xy_to_rs({4,-4}), Hexmap::hex(3,7));
    TEST_EQUAL(rs_to_xy({3,8}), Int2(2,-5));    TEST_EQUAL(xy_to_rs({2,-5}), Hexmap::hex(3,8));
    TEST_EQUAL(rs_to_xy({3,9}), Int2(0,-6));    TEST_EQUAL(xy_to_rs({0,-6}), Hexmap::hex(3,9));
    TEST_EQUAL(rs_to_xy({3,10}), Int2(-2,-5));  TEST_EQUAL(xy_to_rs({-2,-5}), Hexmap::hex(3,10));
    TEST_EQUAL(rs_to_xy({3,11}), Int2(-4,-4));  TEST_EQUAL(xy_to_rs({-4,-4}), Hexmap::hex(3,11));
    TEST_EQUAL(rs_to_xy({3,12}), Int2(-6,-3));  TEST_EQUAL(xy_to_rs({-6,-3}), Hexmap::hex(3,12));
    TEST_EQUAL(rs_to_xy({3,13}), Int2(-6,-1));  TEST_EQUAL(xy_to_rs({-6,-1}), Hexmap::hex(3,13));
    TEST_EQUAL(rs_to_xy({3,14}), Int2(-6,1));   TEST_EQUAL(xy_to_rs({-6,1}), Hexmap::hex(3,14));
    TEST_EQUAL(rs_to_xy({3,15}), Int2(-6,3));   TEST_EQUAL(xy_to_rs({-6,3}), Hexmap::hex(3,15));
    TEST_EQUAL(rs_to_xy({3,16}), Int2(-4,4));   TEST_EQUAL(xy_to_rs({-4,4}), Hexmap::hex(3,16));
    TEST_EQUAL(rs_to_xy({3,17}), Int2(-2,5));   TEST_EQUAL(xy_to_rs({-2,5}), Hexmap::hex(3,17));

    //      h
    //    s   i
    //  r   b   j
    //    g   c
    //  q   a   k
    //    f   d
    //  p   e   l
    //    o   m
    //      n

    Hexmap::hex a = {0,0};
    Hexmap::hex b = {1,0};
    Hexmap::hex c = {1,1};
    Hexmap::hex d = {1,2};
    Hexmap::hex e = {1,3};
    Hexmap::hex f = {1,4};
    Hexmap::hex g = {1,5};
    Hexmap::hex h = {2,0};
    Hexmap::hex i = {2,1};
    Hexmap::hex j = {2,2};
    Hexmap::hex k = {2,3};
    Hexmap::hex l = {2,4};
    Hexmap::hex m = {2,5};
    Hexmap::hex n = {2,6};
    Hexmap::hex o = {2,7};
    Hexmap::hex p = {2,8};
    Hexmap::hex q = {2,9};
    Hexmap::hex r = {2,10};
    Hexmap::hex s = {2,11};

    TEST(Hexmap::is_adjacent(a, a));    TEST_EQUAL(Hexmap::distance(a, a), 0);
    TEST(Hexmap::is_adjacent(a, b));    TEST_EQUAL(Hexmap::distance(a, b), 1);
    TEST(Hexmap::is_adjacent(a, c));    TEST_EQUAL(Hexmap::distance(a, c), 1);
    TEST(Hexmap::is_adjacent(a, d));    TEST_EQUAL(Hexmap::distance(a, d), 1);
    TEST(Hexmap::is_adjacent(a, e));    TEST_EQUAL(Hexmap::distance(a, e), 1);
    TEST(Hexmap::is_adjacent(a, f));    TEST_EQUAL(Hexmap::distance(a, f), 1);
    TEST(Hexmap::is_adjacent(a, g));    TEST_EQUAL(Hexmap::distance(a, g), 1);
    TEST(! Hexmap::is_adjacent(a, h));  TEST_EQUAL(Hexmap::distance(a, h), 2);
    TEST(! Hexmap::is_adjacent(a, i));  TEST_EQUAL(Hexmap::distance(a, i), 2);
    TEST(! Hexmap::is_adjacent(a, j));  TEST_EQUAL(Hexmap::distance(a, j), 2);
    TEST(! Hexmap::is_adjacent(a, k));  TEST_EQUAL(Hexmap::distance(a, k), 2);
    TEST(! Hexmap::is_adjacent(a, l));  TEST_EQUAL(Hexmap::distance(a, l), 2);
    TEST(! Hexmap::is_adjacent(a, m));  TEST_EQUAL(Hexmap::distance(a, m), 2);
    TEST(! Hexmap::is_adjacent(a, n));  TEST_EQUAL(Hexmap::distance(a, n), 2);
    TEST(! Hexmap::is_adjacent(a, o));  TEST_EQUAL(Hexmap::distance(a, o), 2);
    TEST(! Hexmap::is_adjacent(a, p));  TEST_EQUAL(Hexmap::distance(a, p), 2);
    TEST(! Hexmap::is_adjacent(a, q));  TEST_EQUAL(Hexmap::distance(a, q), 2);
    TEST(! Hexmap::is_adjacent(a, r));  TEST_EQUAL(Hexmap::distance(a, r), 2);
    TEST(! Hexmap::is_adjacent(a, s));  TEST_EQUAL(Hexmap::distance(a, s), 2);
    TEST(Hexmap::is_adjacent(b, a));    TEST_EQUAL(Hexmap::distance(b, a), 1);
    TEST(Hexmap::is_adjacent(b, b));    TEST_EQUAL(Hexmap::distance(b, b), 0);
    TEST(Hexmap::is_adjacent(b, c));    TEST_EQUAL(Hexmap::distance(b, c), 1);
    TEST(! Hexmap::is_adjacent(b, d));  TEST_EQUAL(Hexmap::distance(b, d), 2);
    TEST(! Hexmap::is_adjacent(b, e));  TEST_EQUAL(Hexmap::distance(b, e), 2);
    TEST(! Hexmap::is_adjacent(b, f));  TEST_EQUAL(Hexmap::distance(b, f), 2);
    TEST(Hexmap::is_adjacent(b, g));    TEST_EQUAL(Hexmap::distance(b, g), 1);
    TEST(Hexmap::is_adjacent(b, h));    TEST_EQUAL(Hexmap::distance(b, h), 1);
    TEST(Hexmap::is_adjacent(b, i));    TEST_EQUAL(Hexmap::distance(b, i), 1);
    TEST(! Hexmap::is_adjacent(b, j));  TEST_EQUAL(Hexmap::distance(b, j), 2);
    TEST(! Hexmap::is_adjacent(b, k));  TEST_EQUAL(Hexmap::distance(b, k), 2);
    TEST(! Hexmap::is_adjacent(b, l));  TEST_EQUAL(Hexmap::distance(b, l), 3);
    TEST(! Hexmap::is_adjacent(b, m));  TEST_EQUAL(Hexmap::distance(b, m), 3);
    TEST(! Hexmap::is_adjacent(b, n));  TEST_EQUAL(Hexmap::distance(b, n), 3);
    TEST(! Hexmap::is_adjacent(b, o));  TEST_EQUAL(Hexmap::distance(b, o), 3);
    TEST(! Hexmap::is_adjacent(b, p));  TEST_EQUAL(Hexmap::distance(b, p), 3);
    TEST(! Hexmap::is_adjacent(b, q));  TEST_EQUAL(Hexmap::distance(b, q), 2);
    TEST(! Hexmap::is_adjacent(b, r));  TEST_EQUAL(Hexmap::distance(b, r), 2);
    TEST(Hexmap::is_adjacent(b, s));    TEST_EQUAL(Hexmap::distance(b, s), 1);
    TEST(Hexmap::is_adjacent(c, a));    TEST_EQUAL(Hexmap::distance(c, a), 1);
    TEST(Hexmap::is_adjacent(c, b));    TEST_EQUAL(Hexmap::distance(c, b), 1);
    TEST(Hexmap::is_adjacent(c, c));    TEST_EQUAL(Hexmap::distance(c, c), 0);
    TEST(Hexmap::is_adjacent(c, d));    TEST_EQUAL(Hexmap::distance(c, d), 1);
    TEST(! Hexmap::is_adjacent(c, e));  TEST_EQUAL(Hexmap::distance(c, e), 2);
    TEST(! Hexmap::is_adjacent(c, f));  TEST_EQUAL(Hexmap::distance(c, f), 2);
    TEST(! Hexmap::is_adjacent(c, g));  TEST_EQUAL(Hexmap::distance(c, g), 2);
    TEST(! Hexmap::is_adjacent(c, h));  TEST_EQUAL(Hexmap::distance(c, h), 2);
    TEST(Hexmap::is_adjacent(c, i));    TEST_EQUAL(Hexmap::distance(c, i), 1);
    TEST(Hexmap::is_adjacent(c, j));    TEST_EQUAL(Hexmap::distance(c, j), 1);
    TEST(Hexmap::is_adjacent(c, k));    TEST_EQUAL(Hexmap::distance(c, k), 1);
    TEST(! Hexmap::is_adjacent(c, l));  TEST_EQUAL(Hexmap::distance(c, l), 2);
    TEST(! Hexmap::is_adjacent(c, m));  TEST_EQUAL(Hexmap::distance(c, m), 2);
    TEST(! Hexmap::is_adjacent(c, n));  TEST_EQUAL(Hexmap::distance(c, n), 3);
    TEST(! Hexmap::is_adjacent(c, o));  TEST_EQUAL(Hexmap::distance(c, o), 3);
    TEST(! Hexmap::is_adjacent(c, p));  TEST_EQUAL(Hexmap::distance(c, p), 3);
    TEST(! Hexmap::is_adjacent(c, q));  TEST_EQUAL(Hexmap::distance(c, q), 3);
    TEST(! Hexmap::is_adjacent(c, r));  TEST_EQUAL(Hexmap::distance(c, r), 3);
    TEST(! Hexmap::is_adjacent(c, s));  TEST_EQUAL(Hexmap::distance(c, s), 2);
    TEST(Hexmap::is_adjacent(d, a));    TEST_EQUAL(Hexmap::distance(d, a), 1);
    TEST(! Hexmap::is_adjacent(d, b));  TEST_EQUAL(Hexmap::distance(d, b), 2);
    TEST(Hexmap::is_adjacent(d, c));    TEST_EQUAL(Hexmap::distance(d, c), 1);
    TEST(Hexmap::is_adjacent(d, d));    TEST_EQUAL(Hexmap::distance(d, d), 0);
    TEST(Hexmap::is_adjacent(d, e));    TEST_EQUAL(Hexmap::distance(d, e), 1);
    TEST(! Hexmap::is_adjacent(d, f));  TEST_EQUAL(Hexmap::distance(d, f), 2);
    TEST(! Hexmap::is_adjacent(d, g));  TEST_EQUAL(Hexmap::distance(d, g), 2);
    TEST(! Hexmap::is_adjacent(d, h));  TEST_EQUAL(Hexmap::distance(d, h), 3);
    TEST(! Hexmap::is_adjacent(d, i));  TEST_EQUAL(Hexmap::distance(d, i), 2);
    TEST(! Hexmap::is_adjacent(d, j));  TEST_EQUAL(Hexmap::distance(d, j), 2);
    TEST(Hexmap::is_adjacent(d, k));    TEST_EQUAL(Hexmap::distance(d, k), 1);
    TEST(Hexmap::is_adjacent(d, l));    TEST_EQUAL(Hexmap::distance(d, l), 1);
    TEST(Hexmap::is_adjacent(d, m));    TEST_EQUAL(Hexmap::distance(d, m), 1);
    TEST(! Hexmap::is_adjacent(d, n));  TEST_EQUAL(Hexmap::distance(d, n), 2);
    TEST(! Hexmap::is_adjacent(d, o));  TEST_EQUAL(Hexmap::distance(d, o), 2);
    TEST(! Hexmap::is_adjacent(d, p));  TEST_EQUAL(Hexmap::distance(d, p), 3);
    TEST(! Hexmap::is_adjacent(d, q));  TEST_EQUAL(Hexmap::distance(d, q), 3);
    TEST(! Hexmap::is_adjacent(d, r));  TEST_EQUAL(Hexmap::distance(d, r), 3);
    TEST(! Hexmap::is_adjacent(d, s));  TEST_EQUAL(Hexmap::distance(d, s), 3);
    TEST(Hexmap::is_adjacent(e, a));    TEST_EQUAL(Hexmap::distance(e, a), 1);
    TEST(! Hexmap::is_adjacent(e, b));  TEST_EQUAL(Hexmap::distance(e, b), 2);
    TEST(! Hexmap::is_adjacent(e, c));  TEST_EQUAL(Hexmap::distance(e, c), 2);
    TEST(Hexmap::is_adjacent(e, d));    TEST_EQUAL(Hexmap::distance(e, d), 1);
    TEST(Hexmap::is_adjacent(e, e));    TEST_EQUAL(Hexmap::distance(e, e), 0);
    TEST(Hexmap::is_adjacent(e, f));    TEST_EQUAL(Hexmap::distance(e, f), 1);
    TEST(! Hexmap::is_adjacent(e, g));  TEST_EQUAL(Hexmap::distance(e, g), 2);
    TEST(! Hexmap::is_adjacent(e, h));  TEST_EQUAL(Hexmap::distance(e, h), 3);
    TEST(! Hexmap::is_adjacent(e, i));  TEST_EQUAL(Hexmap::distance(e, i), 3);
    TEST(! Hexmap::is_adjacent(e, j));  TEST_EQUAL(Hexmap::distance(e, j), 3);
    TEST(! Hexmap::is_adjacent(e, k));  TEST_EQUAL(Hexmap::distance(e, k), 2);
    TEST(! Hexmap::is_adjacent(e, l));  TEST_EQUAL(Hexmap::distance(e, l), 2);
    TEST(Hexmap::is_adjacent(e, m));    TEST_EQUAL(Hexmap::distance(e, m), 1);
    TEST(Hexmap::is_adjacent(e, n));    TEST_EQUAL(Hexmap::distance(e, n), 1);
    TEST(Hexmap::is_adjacent(e, o));    TEST_EQUAL(Hexmap::distance(e, o), 1);
    TEST(! Hexmap::is_adjacent(e, p));  TEST_EQUAL(Hexmap::distance(e, p), 2);
    TEST(! Hexmap::is_adjacent(e, q));  TEST_EQUAL(Hexmap::distance(e, q), 2);
    TEST(! Hexmap::is_adjacent(e, r));  TEST_EQUAL(Hexmap::distance(e, r), 3);
    TEST(! Hexmap::is_adjacent(e, s));  TEST_EQUAL(Hexmap::distance(e, s), 3);
    TEST(Hexmap::is_adjacent(f, a));    TEST_EQUAL(Hexmap::distance(f, a), 1);
    TEST(! Hexmap::is_adjacent(f, b));  TEST_EQUAL(Hexmap::distance(f, b), 2);
    TEST(! Hexmap::is_adjacent(f, c));  TEST_EQUAL(Hexmap::distance(f, c), 2);
    TEST(! Hexmap::is_adjacent(f, d));  TEST_EQUAL(Hexmap::distance(f, d), 2);
    TEST(Hexmap::is_adjacent(f, e));    TEST_EQUAL(Hexmap::distance(f, e), 1);
    TEST(Hexmap::is_adjacent(f, f));    TEST_EQUAL(Hexmap::distance(f, f), 0);
    TEST(Hexmap::is_adjacent(f, g));    TEST_EQUAL(Hexmap::distance(f, g), 1);
    TEST(! Hexmap::is_adjacent(f, h));  TEST_EQUAL(Hexmap::distance(f, h), 3);
    TEST(! Hexmap::is_adjacent(f, i));  TEST_EQUAL(Hexmap::distance(f, i), 3);
    TEST(! Hexmap::is_adjacent(f, j));  TEST_EQUAL(Hexmap::distance(f, j), 3);
    TEST(! Hexmap::is_adjacent(f, k));  TEST_EQUAL(Hexmap::distance(f, k), 3);
    TEST(! Hexmap::is_adjacent(f, l));  TEST_EQUAL(Hexmap::distance(f, l), 3);
    TEST(! Hexmap::is_adjacent(f, m));  TEST_EQUAL(Hexmap::distance(f, m), 2);
    TEST(! Hexmap::is_adjacent(f, n));  TEST_EQUAL(Hexmap::distance(f, n), 2);
    TEST(Hexmap::is_adjacent(f, o));    TEST_EQUAL(Hexmap::distance(f, o), 1);
    TEST(Hexmap::is_adjacent(f, p));    TEST_EQUAL(Hexmap::distance(f, p), 1);
    TEST(Hexmap::is_adjacent(f, q));    TEST_EQUAL(Hexmap::distance(f, q), 1);
    TEST(! Hexmap::is_adjacent(f, r));  TEST_EQUAL(Hexmap::distance(f, r), 2);
    TEST(! Hexmap::is_adjacent(f, s));  TEST_EQUAL(Hexmap::distance(f, s), 2);
    TEST(Hexmap::is_adjacent(g, a));    TEST_EQUAL(Hexmap::distance(g, a), 1);
    TEST(Hexmap::is_adjacent(g, b));    TEST_EQUAL(Hexmap::distance(g, b), 1);
    TEST(! Hexmap::is_adjacent(g, c));  TEST_EQUAL(Hexmap::distance(g, c), 2);
    TEST(! Hexmap::is_adjacent(g, d));  TEST_EQUAL(Hexmap::distance(g, d), 2);
    TEST(! Hexmap::is_adjacent(g, e));  TEST_EQUAL(Hexmap::distance(g, e), 2);
    TEST(Hexmap::is_adjacent(g, f));    TEST_EQUAL(Hexmap::distance(g, f), 1);
    TEST(Hexmap::is_adjacent(g, g));    TEST_EQUAL(Hexmap::distance(g, g), 0);
    TEST(! Hexmap::is_adjacent(g, h));  TEST_EQUAL(Hexmap::distance(g, h), 2);
    TEST(! Hexmap::is_adjacent(g, i));  TEST_EQUAL(Hexmap::distance(g, i), 2);
    TEST(! Hexmap::is_adjacent(g, j));  TEST_EQUAL(Hexmap::distance(g, j), 3);
    TEST(! Hexmap::is_adjacent(g, k));  TEST_EQUAL(Hexmap::distance(g, k), 3);
    TEST(! Hexmap::is_adjacent(g, l));  TEST_EQUAL(Hexmap::distance(g, l), 3);
    TEST(! Hexmap::is_adjacent(g, m));  TEST_EQUAL(Hexmap::distance(g, m), 3);
    TEST(! Hexmap::is_adjacent(g, n));  TEST_EQUAL(Hexmap::distance(g, n), 3);
    TEST(! Hexmap::is_adjacent(g, o));  TEST_EQUAL(Hexmap::distance(g, o), 2);
    TEST(! Hexmap::is_adjacent(g, p));  TEST_EQUAL(Hexmap::distance(g, p), 2);
    TEST(Hexmap::is_adjacent(g, q));    TEST_EQUAL(Hexmap::distance(g, q), 1);
    TEST(Hexmap::is_adjacent(g, r));    TEST_EQUAL(Hexmap::distance(g, r), 1);
    TEST(Hexmap::is_adjacent(g, s));    TEST_EQUAL(Hexmap::distance(g, s), 1);

    TEST_EQUAL(format_range(Hexmap::neighbours({0,0})), "[[1,0],[1,1],[1,2],[1,3],[1,4],[1,5]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({1,0})), "[[2,0],[2,1],[1,1],[0,0],[1,5],[2,11]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({1,1})), "[[2,1],[2,2],[2,3],[1,2],[0,0],[1,0]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({1,2})), "[[1,1],[2,3],[2,4],[2,5],[1,3],[0,0]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({1,3})), "[[0,0],[1,2],[2,5],[2,6],[2,7],[1,4]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({1,4})), "[[1,5],[0,0],[1,3],[2,7],[2,8],[2,9]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({1,5})), "[[2,11],[1,0],[0,0],[1,4],[2,9],[2,10]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({2,0})), "[[3,0],[3,1],[2,1],[1,0],[2,11],[3,17]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({2,1})), "[[3,1],[3,2],[2,2],[1,1],[1,0],[2,0]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({2,2})), "[[3,2],[3,3],[3,4],[2,3],[1,1],[2,1]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({2,3})), "[[2,2],[3,4],[3,5],[2,4],[1,2],[1,1]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({2,4})), "[[2,3],[3,5],[3,6],[3,7],[2,5],[1,2]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({2,5})), "[[1,2],[2,4],[3,7],[3,8],[2,6],[1,3]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({2,6})), "[[1,3],[2,5],[3,8],[3,9],[3,10],[2,7]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({2,7})), "[[1,4],[1,3],[2,6],[3,10],[3,11],[2,8]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({2,8})), "[[2,9],[1,4],[2,7],[3,11],[3,12],[3,13]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({2,9})), "[[2,10],[1,5],[1,4],[2,8],[3,13],[3,14]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({2,10})), "[[3,16],[2,11],[1,5],[2,9],[3,14],[3,15]]");
    TEST_EQUAL(format_range(Hexmap::neighbours({2,11})), "[[3,17],[2,0],[1,0],[1,5],[2,10],[3,16]]");

    TEST_EQUAL(Hexmap::area(0), 0);
    TEST_EQUAL(Hexmap::area(1), 1);
    TEST_EQUAL(Hexmap::area(2), 7);
    TEST_EQUAL(Hexmap::area(3), 19);
    TEST_EQUAL(Hexmap::area(4), 37);
    TEST_EQUAL(Hexmap::area(5), 61);
    TEST_EQUAL(Hexmap::area(6), 91);

    TEST_EQUAL(Hexmap::perimeter(0), 0);
    TEST_EQUAL(Hexmap::perimeter(1), 1);
    TEST_EQUAL(Hexmap::perimeter(2), 6);
    TEST_EQUAL(Hexmap::perimeter(3), 12);
    TEST_EQUAL(Hexmap::perimeter(4), 18);
    TEST_EQUAL(Hexmap::perimeter(5), 24);
    TEST_EQUAL(Hexmap::perimeter(6), 30);

    TEST_EQUAL(Hexmap::ring_size(0), 1);
    TEST_EQUAL(Hexmap::ring_size(1), 6);
    TEST_EQUAL(Hexmap::ring_size(2), 12);
    TEST_EQUAL(Hexmap::ring_size(3), 18);
    TEST_EQUAL(Hexmap::ring_size(4), 24);
    TEST_EQUAL(Hexmap::ring_size(5), 30);
    TEST_EQUAL(Hexmap::ring_size(6), 36);

    TEST_EQUAL(Hexmap::neighbour({0,0}, Hexmap::side::north), Hexmap::hex(1,0));
    TEST_EQUAL(Hexmap::neighbour({0,0}, Hexmap::side::northeast), Hexmap::hex(1,1));
    TEST_EQUAL(Hexmap::neighbour({0,0}, Hexmap::side::southeast), Hexmap::hex(1,2));
    TEST_EQUAL(Hexmap::neighbour({0,0}, Hexmap::side::south), Hexmap::hex(1,3));
    TEST_EQUAL(Hexmap::neighbour({0,0}, Hexmap::side::southwest), Hexmap::hex(1,4));
    TEST_EQUAL(Hexmap::neighbour({0,0}, Hexmap::side::northwest), Hexmap::hex(1,5));
    TEST_EQUAL(Hexmap::neighbour({2,11}, Hexmap::side::north), Hexmap::hex(3,17));
    TEST_EQUAL(Hexmap::neighbour({2,11}, Hexmap::side::northeast), Hexmap::hex(2,0));
    TEST_EQUAL(Hexmap::neighbour({2,11}, Hexmap::side::southeast), Hexmap::hex(1,0));
    TEST_EQUAL(Hexmap::neighbour({2,11}, Hexmap::side::south), Hexmap::hex(1,5));
    TEST_EQUAL(Hexmap::neighbour({2,11}, Hexmap::side::southwest), Hexmap::hex(2,10));
    TEST_EQUAL(Hexmap::neighbour({2,11}, Hexmap::side::northwest), Hexmap::hex(3,16));

    TEST_EQUAL(Hexmap::principal({0,-2}), Hexmap::hex(0,0));
    TEST_EQUAL(Hexmap::principal({0,-1}), Hexmap::hex(0,0));
    TEST_EQUAL(Hexmap::principal({0,0}), Hexmap::hex(0,0));
    TEST_EQUAL(Hexmap::principal({0,1}), Hexmap::hex(0,0));
    TEST_EQUAL(Hexmap::principal({0,2}), Hexmap::hex(0,0));
    TEST_EQUAL(Hexmap::principal({1,-7}), Hexmap::hex(1,5));
    TEST_EQUAL(Hexmap::principal({1,-6}), Hexmap::hex(1,0));
    TEST_EQUAL(Hexmap::principal({1,-5}), Hexmap::hex(1,1));
    TEST_EQUAL(Hexmap::principal({1,-4}), Hexmap::hex(1,2));
    TEST_EQUAL(Hexmap::principal({1,-3}), Hexmap::hex(1,3));
    TEST_EQUAL(Hexmap::principal({1,-2}), Hexmap::hex(1,4));
    TEST_EQUAL(Hexmap::principal({1,-1}), Hexmap::hex(1,5));
    TEST_EQUAL(Hexmap::principal({1,0}), Hexmap::hex(1,0));
    TEST_EQUAL(Hexmap::principal({1,1}), Hexmap::hex(1,1));
    TEST_EQUAL(Hexmap::principal({1,2}), Hexmap::hex(1,2));
    TEST_EQUAL(Hexmap::principal({1,3}), Hexmap::hex(1,3));
    TEST_EQUAL(Hexmap::principal({1,4}), Hexmap::hex(1,4));
    TEST_EQUAL(Hexmap::principal({1,5}), Hexmap::hex(1,5));
    TEST_EQUAL(Hexmap::principal({1,6}), Hexmap::hex(1,0));
    TEST_EQUAL(Hexmap::principal({1,7}), Hexmap::hex(1,1));
    TEST_EQUAL(Hexmap::principal({2,-25}), Hexmap::hex(2,11));
    TEST_EQUAL(Hexmap::principal({2,-20}), Hexmap::hex(2,4));
    TEST_EQUAL(Hexmap::principal({2,-15}), Hexmap::hex(2,9));
    TEST_EQUAL(Hexmap::principal({2,-10}), Hexmap::hex(2,2));
    TEST_EQUAL(Hexmap::principal({2,-5}), Hexmap::hex(2,7));
    TEST_EQUAL(Hexmap::principal({2,0}), Hexmap::hex(2,0));
    TEST_EQUAL(Hexmap::principal({2,5}), Hexmap::hex(2,5));
    TEST_EQUAL(Hexmap::principal({2,10}), Hexmap::hex(2,10));
    TEST_EQUAL(Hexmap::principal({2,15}), Hexmap::hex(2,3));
    TEST_EQUAL(Hexmap::principal({2,20}), Hexmap::hex(2,8));
    TEST_EQUAL(Hexmap::principal({2,25}), Hexmap::hex(2,1));
    TEST_EQUAL(Hexmap::principal({-1,-15}), Hexmap::hex(1,0));
    TEST_EQUAL(Hexmap::principal({-1,-10}), Hexmap::hex(1,5));
    TEST_EQUAL(Hexmap::principal({-1,-5}), Hexmap::hex(1,4));
    TEST_EQUAL(Hexmap::principal({-1,0}), Hexmap::hex(1,3));
    TEST_EQUAL(Hexmap::principal({-1,5}), Hexmap::hex(1,2));
    TEST_EQUAL(Hexmap::principal({-1,10}), Hexmap::hex(1,1));
    TEST_EQUAL(Hexmap::principal({-1,15}), Hexmap::hex(1,0));
    TEST_EQUAL(Hexmap::principal({-2,-25}), Hexmap::hex(2,5));
    TEST_EQUAL(Hexmap::principal({-2,-20}), Hexmap::hex(2,10));
    TEST_EQUAL(Hexmap::principal({-2,-15}), Hexmap::hex(2,3));
    TEST_EQUAL(Hexmap::principal({-2,-10}), Hexmap::hex(2,8));
    TEST_EQUAL(Hexmap::principal({-2,-5}), Hexmap::hex(2,1));
    TEST_EQUAL(Hexmap::principal({-2,0}), Hexmap::hex(2,6));
    TEST_EQUAL(Hexmap::principal({-2,5}), Hexmap::hex(2,11));
    TEST_EQUAL(Hexmap::principal({-2,10}), Hexmap::hex(2,4));
    TEST_EQUAL(Hexmap::principal({-2,15}), Hexmap::hex(2,9));
    TEST_EQUAL(Hexmap::principal({-2,20}), Hexmap::hex(2,2));
    TEST_EQUAL(Hexmap::principal({-2,25}), Hexmap::hex(2,7));

}

void test_rs_game_hexmap_adjacency_properties() {

    Hexmap map;
    Hexmap::hexlist list;

    TEST_EQUAL(map.rings(), 0);
    TEST_EQUAL(map.size(), 0);

    TEST_EQUAL(format_range(map.all()), "[]");
    TEST_EQUAL(format_range(map.border_in()), "[]");
    TEST_EQUAL(format_range(map.border_out()), "[]");

    //      __      //
    //   __/10\__   //
    //  /15\__/11\  //
    //  \__/00\__/  //
    //  /14\__/12\  //
    //  \__/13\__/  //
    //     \__/     //

    TRY(map.set({0,0}, "00"));
    TRY(map.set({1,0}, "10"));
    TRY(map.set({1,1}, "11"));
    TRY(map.set({1,2}, "12"));
    TRY(map.set({1,3}, "13"));
    TRY(map.set({1,4}, "14"));
    TRY(map.set({1,5}, "15"));

    TEST_EQUAL(map.rings(), 2);
    TEST_EQUAL(map.size(), 7);

    TEST_EQUAL(map.label({0,0}), "00");
    TEST_EQUAL(map.label({1,0}), "10");
    TEST_EQUAL(map.label({1,1}), "11");
    TEST_EQUAL(map.label({1,2}), "12");
    TEST_EQUAL(map.label({1,3}), "13");
    TEST_EQUAL(map.label({1,4}), "14");
    TEST_EQUAL(map.label({1,5}), "15");

    TRY(list = map.all());
    TEST_EQUAL(list.size(), 7u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list), "[[0,0],[1,0],[1,1],[1,2],[1,3],[1,4],[1,5]]");
    TRY(list = map.border_in());
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(list.size(), 6u);
    TEST_EQUAL(format_range(list), "[[1,0],[1,1],[1,2],[1,3],[1,4],[1,5]]");
    TRY(list = map.border_out());
    TEST_EQUAL(list.size(), 12u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list), "[[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6],[2,7],[2,8],[2,9],[2,10],[2,11]]");

    TRY(list = map.reachable({0,0}));
    TEST_EQUAL(list.size(), 7u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list), "[[0,0],[1,0],[1,1],[1,2],[1,3],[1,4],[1,5]]");
    TRY(list = map.reachable({1,0}));
    TEST_EQUAL(list.size(), 7u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list), "[[0,0],[1,0],[1,1],[1,2],[1,3],[1,4],[1,5]]");
    TRY(list = map.reachable({2,0}));
    TEST_EQUAL(list.size(), 7u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list), "[[0,0],[1,0],[1,1],[1,2],[1,3],[1,4],[1,5]]");
    TRY(list = map.reachable({3,0}));
    TEST(list.empty());

    //      __         //
    //     /20\__      //
    //     \__/21\__   //
    //   __/10\__/22\  //
    //  /15\__/11\__/  //
    //  \__/00\__/23\  //
    //  /14\__/12\__/  //
    //  \__/13\__/24\  //
    //     \__/25\__/  //
    //     /26\__/     //
    //     \__/        //

    TRY(map.set({2,0}, "20"));
    TRY(map.set({2,1}, "21"));
    TRY(map.set({2,2}, "22"));
    TRY(map.set({2,3}, "23"));
    TRY(map.set({2,4}, "24"));
    TRY(map.set({2,5}, "25"));
    TRY(map.set({2,6}, "26"));

    TEST_EQUAL(map.rings(), 3);
    TEST_EQUAL(map.size(), 14);

    TRY(list = map.all());
    TEST_EQUAL(list.size(), 14u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[0,0],"
        "[1,0],[1,1],[1,2],[1,3],[1,4],[1,5],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.border_in());
    TEST_EQUAL(list.size(), 11u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,0],[1,3],[1,4],[1,5],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.border_out());
    TEST_EQUAL(list.size(), 17u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[2,7],[2,8],[2,9],[2,10],[2,11],"
        "[3,0],[3,1],[3,2],[3,3],[3,4],[3,5],[3,6],[3,7],[3,8],[3,9],[3,10],[3,17]]");

    TEST_EQUAL(format_range(map.neighbours_set({0,0})), "[[1,0],[1,1],[1,2],[1,3],[1,4],[1,5]]");
    TEST_EQUAL(format_range(map.neighbours_unset({0,0})), "[]");
    TEST_EQUAL(format_range(map.neighbours_set({2,0})), "[[2,1],[1,0]]");
    TEST_EQUAL(format_range(map.neighbours_unset({2,0})), "[[3,0],[3,1],[2,11],[3,17]]");
    TEST_EQUAL(format_range(map.neighbours_set({4,0})), "[]");
    TEST_EQUAL(format_range(map.neighbours_unset({4,0})), "[[5,0],[5,1],[4,1],[3,0],[4,23],[5,29]]");

    //      __         //
    //     /20\__      //
    //     \__/21\__   //
    //   __/10\__/22\  //
    //  /15\__/11\__/  //
    //  \__/  \__/23\  //
    //  /14\__/12\__/  //
    //  \__/13\__/24\  //
    //     \__/25\__/  //
    //     /26\__/     //
    //     \__/        //

    TEST_EQUAL(map.size(), 14);
    TRY(map.set({0,0}, ""));

    TEST_EQUAL(map.rings(), 3);
    TEST_EQUAL(map.size(), 13);

    TRY(list = map.all());
    TEST_EQUAL(list.size(), 13u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,0],[1,1],[1,2],[1,3],[1,4],[1,5],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.border_in());
    TEST_EQUAL(list.size(), 13u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,0],[1,1],[1,2],[1,3],[1,4],[1,5],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.border_out());
    TEST_EQUAL(list.size(), 18u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[0,0],"
        "[2,7],[2,8],[2,9],[2,10],[2,11],"
        "[3,0],[3,1],[3,2],[3,3],[3,4],[3,5],[3,6],[3,7],[3,8],[3,9],[3,10],[3,17]]");

    //      __         //
    //     /20\__      //
    //     \__/21\__   //
    //   __   \__/22\  //
    //  /15\  /11\__/  //
    //  \__/  \__/23\  //
    //  /14\  /12\__/  //
    //  \__/  \__/24\  //
    //      __/25\__/  //
    //     /26\__/     //
    //     \__/        //

    TRY(map.erase({1,0}));
    TRY(map.erase({1,3}));

    TEST_EQUAL(map.rings(), 3);
    TEST_EQUAL(map.size(), 11);

    TRY(list = map.all());
    TEST_EQUAL(list.size(), 11u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,1],[1,2],[1,4],[1,5],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.border_in());
    TEST_EQUAL(list.size(), 11u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,1],[1,2],[1,4],[1,5],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.border_out());
    TEST_EQUAL(list.size(), 20u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[0,0],"
        "[1,0],[1,3],"
        "[2,7],[2,8],[2,9],[2,10],[2,11],"
        "[3,0],[3,1],[3,2],[3,3],[3,4],[3,5],[3,6],[3,7],[3,8],[3,9],[3,10],[3,17]]");

    TRY(list = map.reachable({0,0}));
    TEST_EQUAL(list.size(), 11u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,1],[1,2],[1,4],[1,5],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.reachable({1,1}));
    TEST_EQUAL(list.size(), 9u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,1],[1,2],"
        "[2,0],[2,1],[2,2],[2,3],[2,4],[2,5],[2,6]]");
    TRY(list = map.reachable({1,5}));
    TEST_EQUAL(list.size(), 2u);
    TEST(std::is_sorted(list.begin(), list.end(), cmp_hex));
    TEST_EQUAL(format_range(list),
        "[[1,4],[1,5]]");

}

void test_rs_game_hexmap_ascii_art() {

    Hexmap map;
    Hexmap::hexlist list;

    TEST_EQUAL(map.str(),
        R"( __ )" "\n"
        R"(/  \)" "\n"
        R"(\__/)" "\n"
    );

    TRY(map.set({0,0}, "00"));
    TRY(map.set({1,0}, "10"));
    TRY(map.set({1,1}, "11"));
    TRY(map.set({1,2}, "12"));
    TRY(map.set({1,3}, "13"));
    TRY(map.set({1,4}, "14"));
    TRY(map.set({1,5}, "15"));

    TEST_EQUAL(map.str(),
        R"(    __    )" "\n"
        R"( __/10\__ )" "\n"
        R"(/15\__/11\)" "\n"
        R"(\__/00\__/)" "\n"
        R"(/14\__/12\)" "\n"
        R"(\__/13\__/)" "\n"
        R"(   \__/   )" "\n"
    );

    TRY(map.set_radius(3));

    TEST_EQUAL(map.str(),
        R"(       __       )" "\n"
        R"(    __/  \__    )" "\n"
        R"( __/  \__/  \__ )" "\n"
        R"(/  \__/10\__/  \)" "\n"
        R"(\__/15\__/11\__/)" "\n"
        R"(/  \__/00\__/  \)" "\n"
        R"(\__/14\__/12\__/)" "\n"
        R"(/  \__/13\__/  \)" "\n"
        R"(\__/  \__/  \__/)" "\n"
        R"(   \__/  \__/   )" "\n"
        R"(      \__/      )" "\n"
    );

    TRY(map.set_radius(3));
    TRY(map.set_width(4));

    TEST_EQUAL(map.str(),
        R"(           ____           )" "\n"
        R"(      ____/    \____      )" "\n"
        R"( ____/    \____/    \____ )" "\n"
        R"(/    \____/10  \____/    \)" "\n"
        R"(\____/15  \____/11  \____/)" "\n"
        R"(/    \____/00  \____/    \)" "\n"
        R"(\____/14  \____/12  \____/)" "\n"
        R"(/    \____/13  \____/    \)" "\n"
        R"(\____/    \____/    \____/)" "\n"
        R"(     \____/    \____/     )" "\n"
        R"(          \____/          )" "\n"
    );

    TRY(map.set({2,0}, "20*"));
    TRY(map.set({2,1}, "21*"));
    TRY(map.set({2,2}, "22*"));
    TRY(map.set({2,3}, "23*"));
    TRY(map.set({2,4}, "24*"));
    TRY(map.set({2,5}, "25*"));
    TRY(map.set({2,6}, "26*"));

    TRY(map.set_radius());
    TRY(map.set_width());

    TEST_EQUAL(map.str(),
        R"(         ___         )" "\n"
        R"(     ___/20*\___     )" "\n"
        R"( ___/   \___/21*\___ )" "\n"
        R"(/   \___/10 \___/22*\)" "\n"
        R"(\___/15 \___/11 \___/)" "\n"
        R"(/   \___/00 \___/23*\)" "\n"
        R"(\___/14 \___/12 \___/)" "\n"
        R"(/   \___/13 \___/24*\)" "\n"
        R"(\___/   \___/25*\___/)" "\n"
        R"(    \___/26*\___/    )" "\n"
        R"(        \___/        )" "\n"
    );

    TRY(map.set({0,0}, ""));

    TEST_EQUAL(map.str(),
        R"(         ___         )" "\n"
        R"(     ___/20*\___     )" "\n"
        R"( ___/   \___/21*\___ )" "\n"
        R"(/   \___/10 \___/22*\)" "\n"
        R"(\___/15 \___/11 \___/)" "\n"
        R"(/   \___/   \___/23*\)" "\n"
        R"(\___/14 \___/12 \___/)" "\n"
        R"(/   \___/13 \___/24*\)" "\n"
        R"(\___/   \___/25*\___/)" "\n"
        R"(    \___/26*\___/    )" "\n"
        R"(        \___/        )" "\n"
    );

    TRY(map.erase({1,0}));
    TRY(map.erase({1,3}));

    TEST_EQUAL(map.str(),
        R"(         ___         )" "\n"
        R"(     ___/20*\___     )" "\n"
        R"( ___/   \___/21*\___ )" "\n"
        R"(/   \___/   \___/22*\)" "\n"
        R"(\___/15 \___/11 \___/)" "\n"
        R"(/   \___/   \___/23*\)" "\n"
        R"(\___/14 \___/12 \___/)" "\n"
        R"(/   \___/   \___/24*\)" "\n"
        R"(\___/   \___/25*\___/)" "\n"
        R"(    \___/26*\___/    )" "\n"
        R"(        \___/        )" "\n"
    );

    TRY(map.clear());

    TRY(map.set({0,0}, "00", {5,3,1}));
    TRY(map.set({1,0}, "10", {2,5,2}));
    TRY(map.set({1,1}, "11", {2,2,5}));
    TRY(map.set({1,2}, "12", {2,5,2}));
    TRY(map.set({1,3}, "13", {2,2,5}));
    TRY(map.set({1,4}, "14", {2,5,2}));
    TRY(map.set({1,5}, "15", {2,2,5}));

    TEST_EQUAL(map.str(),
        R"(    __    )" "\n"
        R"( __/10\__ )" "\n"
        R"(/15\__/11\)" "\n"
        R"(\__/00\__/)" "\n"
        R"(/14\__/12\)" "\n"
        R"(\__/13\__/)" "\n"
        R"(   \__/   )" "\n"
    );

    TRY(map.set_xterm(Hexmap::xt::yes));

    TEST_EQUAL(map.str(),
        "    __    \n"
        " __/\x1b[1m\x1b[38;5;120m10\x1b[0m\\__ \n"
        "/\x1b[1m\x1b[38;5;105m15\x1b[0m\\__/\x1b[1m\x1b[38;5;105m11\x1b[0m\\\n"
        "\\__/\x1b[1m\x1b[38;5;215m00\x1b[0m\\__/\n"
        "/\x1b[1m\x1b[38;5;120m14\x1b[0m\\__/\x1b[1m\x1b[38;5;120m12\x1b[0m\\\n"
        "\\__/\x1b[1m\x1b[38;5;105m13\x1b[0m\\__/\n"
        "   \\__/   \n"
    );

    // std::cout << map;

}
