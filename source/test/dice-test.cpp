#include "rs-game/dice.hpp"
#include "rs-sci/rational.hpp"
#include "rs-sci/statistics.hpp"
#include "rs-unit-test.hpp"
#include <random>

using namespace RS::Game;
using namespace RS::Game::Literals;
using namespace RS::Sci;

void test_rs_game_dice_arithmetic() {

    Dice a, b, c;
    Rational r(2, 3);

    TRY(a = Dice(2, 6));
    TRY(b = Dice(3, 10));

    TRY(c = a * 3);  TEST_EQUAL(c.str(), "2d6*3");
    TRY(c = a / 3);  TEST_EQUAL(c.str(), "2d6/3");
    TRY(c = a * r);  TEST_EQUAL(c.str(), "2d6*2/3");
    TRY(c = a / r);  TEST_EQUAL(c.str(), "2d6*3/2");
    TRY(c = a + b);  TEST_EQUAL(c.str(), "3d10+2d6");
    TRY(c = a - b);  TEST_EQUAL(c.str(), "-3d10+2d6");
    TRY(c = b - a);  TEST_EQUAL(c.str(), "3d10-2d6");

}

void test_rs_game_dice_statistics() {

    Dice dice;

    TEST_EQUAL(dice.min(), 0);
    TEST_EQUAL(dice.max(), 0);
    TEST_EQUAL(dice.mean(), 0);
    TEST_EQUAL(dice.sd(), 0);
    TEST_EQUAL(dice.str(), "0");

    TRY(dice += 5);
    TEST_EQUAL(dice.min(), 5);
    TEST_EQUAL(dice.max(), 5);
    TEST_EQUAL(dice.mean(), 5);
    TEST_EQUAL(dice.sd(), 0);
    TEST_EQUAL(dice.str(), "5");

    TRY(dice += Dice(2, 6));
    TEST_EQUAL(dice.min(), 7);
    TEST_EQUAL(dice.max(), 17);
    TEST_EQUAL(dice.mean(), 12);
    TEST_NEAR(dice.sd(), 2.415229, 1e-6);
    TEST_EQUAL(dice.str(), "2d6+5");

    TRY(dice -= 10);
    TEST_EQUAL(dice.min(), -3);
    TEST_EQUAL(dice.max(), 7);
    TEST_EQUAL(dice.mean(), 2);
    TEST_NEAR(dice.sd(), 2.415229, 1e-6);
    TEST_EQUAL(dice.str(), "2d6-5");

    TRY(dice -= Dice(2, 10));
    TEST_EQUAL(dice.min(), -23);
    TEST_EQUAL(dice.max(), 5);
    TEST_EQUAL(dice.mean(), -9);
    TEST_NEAR(dice.sd(), 4.725816, 1e-6);
    TEST_EQUAL(dice.str(), "-2d10+2d6-5");

}

void test_rs_game_dice_parser() {

    Dice dice;

    TRY(dice = Dice(""));
    TEST_EQUAL(dice.str(), "0");
    TEST_EQUAL(dice.min(), 0);
    TEST_EQUAL(dice.max(), 0);
    TEST_EQUAL(dice.mean(), 0);
    TEST_EQUAL(dice.sd(), 0);

    TRY(dice = Dice("5"));
    TEST_EQUAL(dice.str(), "5");
    TEST_EQUAL(dice.min(), 5);
    TEST_EQUAL(dice.max(), 5);
    TEST_EQUAL(dice.mean(), 5);
    TEST_EQUAL(dice.sd(), 0);

    TRY(dice = Dice("2d6"));
    TEST_EQUAL(dice.str(), "2d6");
    TEST_EQUAL(dice.min(), 2);
    TEST_EQUAL(dice.max(), 12);
    TEST_EQUAL(dice.mean(), 7);
    TEST_NEAR(dice.sd(), 2.415229, 1e-6);

    TRY(dice = Dice("2d10+2d6+10"));
    TEST_EQUAL(dice.str(), "2d10+2d6+10");
    TEST_EQUAL(dice.min(), 14);
    TEST_EQUAL(dice.max(), 42);
    TEST_EQUAL(dice.mean(), 28);
    TEST_NEAR(dice.sd(), 4.725816, 1e-6);

    TRY(dice = Dice("2d10-2d6+10"));
    TEST_EQUAL(dice.str(), "2d10-2d6+10");
    TEST_EQUAL(dice.min(), 0);
    TEST_EQUAL(dice.max(), 28);
    TEST_EQUAL(dice.mean(), 14);
    TEST_NEAR(dice.sd(), 4.725816, 1e-6);

    TRY(dice = Dice(" 3*2d10 - 2d6/4 + d8*6/8 + 10\n"));
    TEST_EQUAL(dice.str(), "2d10*3+d8*3/4-2d6/4+10");
    TEST_EQUAL(dice.min(), Rational(55,4));
    TEST_EQUAL(dice.max(), Rational(151,2));
    TEST_EQUAL(dice.mean(), Rational(357,8));
    TEST_NEAR(dice.sd(), 12.321433, 1e-6);

}

void test_rs_game_dice_generation() {

    static constexpr int iterations = 100'000;
    static constexpr double tolerance = 0.05;

    Dice dice;
    std::minstd_rand rng(42);
    Statistics<double> stats;
    Rational x;

    stats = {};
    TRY(dice = Dice(2, 6));
    for (int i = 0; i < iterations; ++i) {
        TRY(x = dice(rng));
        TRY(stats(double(x)));
    }
    TEST_EQUAL(stats.min(), double(dice.min()));
    TEST_EQUAL(stats.max(), double(dice.max()));
    TEST_NEAR(stats.mean(), double(dice.mean()), tolerance);
    TEST_NEAR(stats.sd(), dice.sd(), tolerance);

    stats = {};
    TRY(dice = Dice("2d6"));
    for (int i = 0; i < iterations; ++i) {
        TRY(x = dice(rng));
        TRY(stats(double(x)));
    }
    TEST_EQUAL(stats.min(), double(dice.min()));
    TEST_EQUAL(stats.max(), double(dice.max()));
    TEST_NEAR(stats.mean(), double(dice.mean()), tolerance);
    TEST_NEAR(stats.sd(), dice.sd(), tolerance);

    stats = {};
    TRY(dice = Dice("2d10-2d6+10"));
    for (int i = 0; i < iterations; ++i) {
        TRY(x = dice(rng));
        TRY(stats(double(x)));
    }
    TEST_EQUAL(stats.min(), double(dice.min()));
    TEST_EQUAL(stats.max(), double(dice.max()));
    TEST_NEAR(stats.mean(), double(dice.mean()), tolerance);
    TEST_NEAR(stats.sd(), dice.sd(), tolerance);

}

void test_rs_game_dice_literals() {

    Dice dice;

    TRY(dice = 5_d4);         TEST_EQUAL(dice.str(), "5d4");
    TRY(dice = 5_d6);         TEST_EQUAL(dice.str(), "5d6");
    TRY(dice = 5_d8);         TEST_EQUAL(dice.str(), "5d8");
    TRY(dice = 5_d10);        TEST_EQUAL(dice.str(), "5d10");
    TRY(dice = 5_d12);        TEST_EQUAL(dice.str(), "5d12");
    TRY(dice = 5_d20);        TEST_EQUAL(dice.str(), "5d20");
    TRY(dice = 5_d100);       TEST_EQUAL(dice.str(), "5d100");
    TRY(dice = "5d"_dice);    TEST_EQUAL(dice.str(), "5d6");
    TRY(dice = "5d20"_dice);  TEST_EQUAL(dice.str(), "5d20");

}

void test_rs_game_dice_pdf() {

    Dice d;

    TRY(d = 1_d6);

    TEST_EQUAL(d.pdf(0),  0         );       TEST_EQUAL(d.cdf(0),  0         );       TEST_EQUAL(d.ccdf(0),  1         );
    TEST_NEAR(d.pdf(1),   1.0/6.0,  1e-10);  TEST_NEAR(d.cdf(1),   1.0/6.0,  1e-10);  TEST_EQUAL(d.ccdf(1),  1         );
    TEST_NEAR(d.pdf(2),   1.0/6.0,  1e-10);  TEST_NEAR(d.cdf(2),   2.0/6.0,  1e-10);  TEST_NEAR(d.ccdf(2),   5.0/6.0,  1e-10);
    TEST_NEAR(d.pdf(3),   1.0/6.0,  1e-10);  TEST_NEAR(d.cdf(3),   3.0/6.0,  1e-10);  TEST_NEAR(d.ccdf(3),   4.0/6.0,  1e-10);
    TEST_NEAR(d.pdf(4),   1.0/6.0,  1e-10);  TEST_NEAR(d.cdf(4),   4.0/6.0,  1e-10);  TEST_NEAR(d.ccdf(4),   3.0/6.0,  1e-10);
    TEST_NEAR(d.pdf(5),   1.0/6.0,  1e-10);  TEST_NEAR(d.cdf(5),   5.0/6.0,  1e-10);  TEST_NEAR(d.ccdf(5),   2.0/6.0,  1e-10);
    TEST_NEAR(d.pdf(6),   1.0/6.0,  1e-10);  TEST_EQUAL(d.cdf(6),  1         );       TEST_NEAR(d.ccdf(6),   1.0/6.0,  1e-10);
    TEST_EQUAL(d.pdf(7),  0         );       TEST_EQUAL(d.cdf(7),  1         );       TEST_EQUAL(d.ccdf(7),  0         );

    TRY(d = 3_d6);

    TEST_EQUAL(d.pdf(2),   0            );       TEST_EQUAL(d.cdf(2),   0             );       TEST_EQUAL(d.ccdf(2),   1             );
    TEST_NEAR(d.pdf(3),    1.0/216.0,   1e-10);  TEST_NEAR(d.cdf(3),    1.0/216.0,    1e-10);  TEST_EQUAL(d.ccdf(3),   1             );
    TEST_NEAR(d.pdf(4),    3.0/216.0,   1e-10);  TEST_NEAR(d.cdf(4),    4.0/216.0,    1e-10);  TEST_NEAR(d.ccdf(4),    215.0/216.0,  1e-10);
    TEST_NEAR(d.pdf(5),    6.0/216.0,   1e-10);  TEST_NEAR(d.cdf(5),    10.0/216.0,   1e-10);  TEST_NEAR(d.ccdf(5),    212.0/216.0,  1e-10);
    TEST_NEAR(d.pdf(6),    10.0/216.0,  1e-10);  TEST_NEAR(d.cdf(6),    20.0/216.0,   1e-10);  TEST_NEAR(d.ccdf(6),    206.0/216.0,  1e-10);
    TEST_NEAR(d.pdf(7),    15.0/216.0,  1e-10);  TEST_NEAR(d.cdf(7),    35.0/216.0,   1e-10);  TEST_NEAR(d.ccdf(7),    196.0/216.0,  1e-10);
    TEST_NEAR(d.pdf(8),    21.0/216.0,  1e-10);  TEST_NEAR(d.cdf(8),    56.0/216.0,   1e-10);  TEST_NEAR(d.ccdf(8),    181.0/216.0,  1e-10);
    TEST_NEAR(d.pdf(9),    25.0/216.0,  1e-10);  TEST_NEAR(d.cdf(9),    81.0/216.0,   1e-10);  TEST_NEAR(d.ccdf(9),    160.0/216.0,  1e-10);
    TEST_NEAR(d.pdf(10),   27.0/216.0,  1e-10);  TEST_NEAR(d.cdf(10),   108.0/216.0,  1e-10);  TEST_NEAR(d.ccdf(10),   135.0/216.0,  1e-10);
    TEST_NEAR(d.pdf(11),   27.0/216.0,  1e-10);  TEST_NEAR(d.cdf(11),   135.0/216.0,  1e-10);  TEST_NEAR(d.ccdf(11),   108.0/216.0,  1e-10);
    TEST_NEAR(d.pdf(12),   25.0/216.0,  1e-10);  TEST_NEAR(d.cdf(12),   160.0/216.0,  1e-10);  TEST_NEAR(d.ccdf(12),   81.0/216.0,   1e-10);
    TEST_NEAR(d.pdf(13),   21.0/216.0,  1e-10);  TEST_NEAR(d.cdf(13),   181.0/216.0,  1e-10);  TEST_NEAR(d.ccdf(13),   56.0/216.0,   1e-10);
    TEST_NEAR(d.pdf(14),   15.0/216.0,  1e-10);  TEST_NEAR(d.cdf(14),   196.0/216.0,  1e-10);  TEST_NEAR(d.ccdf(14),   35.0/216.0,   1e-10);
    TEST_NEAR(d.pdf(15),   10.0/216.0,  1e-10);  TEST_NEAR(d.cdf(15),   206.0/216.0,  1e-10);  TEST_NEAR(d.ccdf(15),   20.0/216.0,   1e-10);
    TEST_NEAR(d.pdf(16),   6.0/216.0,   1e-10);  TEST_NEAR(d.cdf(16),   212.0/216.0,  1e-10);  TEST_NEAR(d.ccdf(16),   10.0/216.0,   1e-10);
    TEST_NEAR(d.pdf(17),   3.0/216.0,   1e-10);  TEST_NEAR(d.cdf(17),   215.0/216.0,  1e-10);  TEST_NEAR(d.ccdf(17),   4.0/216.0,    1e-10);
    TEST_NEAR(d.pdf(18),   1.0/216.0,   1e-10);  TEST_EQUAL(d.cdf(18),  1             );       TEST_NEAR(d.ccdf(18),   1.0/216.0,    1e-10);
    TEST_EQUAL(d.pdf(19),  0            );       TEST_EQUAL(d.cdf(19),  1             );       TEST_EQUAL(d.ccdf(19),  0             );

    TRY(d = 2_d6 + 1_d10 - 2);

    TEST_EQUAL(d.pdf(0),   0            );       TEST_EQUAL(d.cdf(0),   0             );       TEST_EQUAL(d.ccdf(0),   1             );
    TEST_NEAR(d.pdf(1),    1.0/360.0,   1e-10);  TEST_NEAR(d.cdf(1),    1.0/360.0,    1e-10);  TEST_EQUAL(d.ccdf(1),   1             );
    TEST_NEAR(d.pdf(2),    3.0/360.0,   1e-10);  TEST_NEAR(d.cdf(2),    4.0/360.0,    1e-10);  TEST_NEAR(d.ccdf(2),    359.0/360.0,  1e-10);
    TEST_NEAR(d.pdf(3),    6.0/360.0,   1e-10);  TEST_NEAR(d.cdf(3),    10.0/360.0,   1e-10);  TEST_NEAR(d.ccdf(3),    356.0/360.0,  1e-10);
    TEST_NEAR(d.pdf(4),    10.0/360.0,  1e-10);  TEST_NEAR(d.cdf(4),    20.0/360.0,   1e-10);  TEST_NEAR(d.ccdf(4),    350.0/360.0,  1e-10);
    TEST_NEAR(d.pdf(5),    15.0/360.0,  1e-10);  TEST_NEAR(d.cdf(5),    35.0/360.0,   1e-10);  TEST_NEAR(d.ccdf(5),    340.0/360.0,  1e-10);
    TEST_NEAR(d.pdf(6),    21.0/360.0,  1e-10);  TEST_NEAR(d.cdf(6),    56.0/360.0,   1e-10);  TEST_NEAR(d.ccdf(6),    325.0/360.0,  1e-10);
    TEST_NEAR(d.pdf(7),    26.0/360.0,  1e-10);  TEST_NEAR(d.cdf(7),    82.0/360.0,   1e-10);  TEST_NEAR(d.ccdf(7),    304.0/360.0,  1e-10);
    TEST_NEAR(d.pdf(8),    30.0/360.0,  1e-10);  TEST_NEAR(d.cdf(8),    112.0/360.0,  1e-10);  TEST_NEAR(d.ccdf(8),    278.0/360.0,  1e-10);
    TEST_NEAR(d.pdf(9),    33.0/360.0,  1e-10);  TEST_NEAR(d.cdf(9),    145.0/360.0,  1e-10);  TEST_NEAR(d.ccdf(9),    248.0/360.0,  1e-10);
    TEST_NEAR(d.pdf(10),   35.0/360.0,  1e-10);  TEST_NEAR(d.cdf(10),   180.0/360.0,  1e-10);  TEST_NEAR(d.ccdf(10),   215.0/360.0,  1e-10);
    TEST_NEAR(d.pdf(11),   35.0/360.0,  1e-10);  TEST_NEAR(d.cdf(11),   215.0/360.0,  1e-10);  TEST_NEAR(d.ccdf(11),   180.0/360.0,  1e-10);
    TEST_NEAR(d.pdf(12),   33.0/360.0,  1e-10);  TEST_NEAR(d.cdf(12),   248.0/360.0,  1e-10);  TEST_NEAR(d.ccdf(12),   145.0/360.0,  1e-10);
    TEST_NEAR(d.pdf(13),   30.0/360.0,  1e-10);  TEST_NEAR(d.cdf(13),   278.0/360.0,  1e-10);  TEST_NEAR(d.ccdf(13),   112.0/360.0,  1e-10);
    TEST_NEAR(d.pdf(14),   26.0/360.0,  1e-10);  TEST_NEAR(d.cdf(14),   304.0/360.0,  1e-10);  TEST_NEAR(d.ccdf(14),   82.0/360.0,   1e-10);
    TEST_NEAR(d.pdf(15),   21.0/360.0,  1e-10);  TEST_NEAR(d.cdf(15),   325.0/360.0,  1e-10);  TEST_NEAR(d.ccdf(15),   56.0/360.0,   1e-10);
    TEST_NEAR(d.pdf(16),   15.0/360.0,  1e-10);  TEST_NEAR(d.cdf(16),   340.0/360.0,  1e-10);  TEST_NEAR(d.ccdf(16),   35.0/360.0,   1e-10);
    TEST_NEAR(d.pdf(17),   10.0/360.0,  1e-10);  TEST_NEAR(d.cdf(17),   350.0/360.0,  1e-10);  TEST_NEAR(d.ccdf(17),   20.0/360.0,   1e-10);
    TEST_NEAR(d.pdf(18),   6.0/360.0,   1e-10);  TEST_NEAR(d.cdf(18),   356.0/360.0,  1e-10);  TEST_NEAR(d.ccdf(18),   10.0/360.0,   1e-10);
    TEST_NEAR(d.pdf(19),   3.0/360.0,   1e-10);  TEST_NEAR(d.cdf(19),   359.0/360.0,  1e-10);  TEST_NEAR(d.ccdf(19),   4.0/360.0,    1e-10);
    TEST_NEAR(d.pdf(20),   1.0/360.0,   1e-10);  TEST_EQUAL(d.cdf(20),  1             );       TEST_NEAR(d.ccdf(20),   1.0/360.0,    1e-10);
    TEST_EQUAL(d.pdf(21),  0            );       TEST_EQUAL(d.cdf(21),  1             );       TEST_EQUAL(d.ccdf(21),  0             );

}
