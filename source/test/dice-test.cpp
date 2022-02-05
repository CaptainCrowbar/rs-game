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

    TEST_EQUAL(d.pdf(0),  0);               TEST_EQUAL(d.cdf(0),  0);               TEST_EQUAL(d.ccdf(0),  1);
    TEST_EQUAL(d.pdf(1),  Rational(1, 6));  TEST_EQUAL(d.cdf(1),  Rational(1, 6));  TEST_EQUAL(d.ccdf(1),  1);
    TEST_EQUAL(d.pdf(2),  Rational(1, 6));  TEST_EQUAL(d.cdf(2),  Rational(2, 6));  TEST_EQUAL(d.ccdf(2),  Rational(5, 6));
    TEST_EQUAL(d.pdf(3),  Rational(1, 6));  TEST_EQUAL(d.cdf(3),  Rational(3, 6));  TEST_EQUAL(d.ccdf(3),  Rational(4, 6));
    TEST_EQUAL(d.pdf(4),  Rational(1, 6));  TEST_EQUAL(d.cdf(4),  Rational(4, 6));  TEST_EQUAL(d.ccdf(4),  Rational(3, 6));
    TEST_EQUAL(d.pdf(5),  Rational(1, 6));  TEST_EQUAL(d.cdf(5),  Rational(5, 6));  TEST_EQUAL(d.ccdf(5),  Rational(2, 6));
    TEST_EQUAL(d.pdf(6),  Rational(1, 6));  TEST_EQUAL(d.cdf(6),  1);               TEST_EQUAL(d.ccdf(6),  Rational(1, 6));
    TEST_EQUAL(d.pdf(7),  0);               TEST_EQUAL(d.cdf(7),  1);               TEST_EQUAL(d.ccdf(7),  0);

    TRY(d = 3_d6);

    TEST_EQUAL(d.pdf(2),   0);                  TEST_EQUAL(d.cdf(2),   0);                   TEST_EQUAL(d.ccdf(2),   1);
    TEST_EQUAL(d.pdf(3),   Rational(1, 216));   TEST_EQUAL(d.cdf(3),   Rational(1, 216));    TEST_EQUAL(d.ccdf(3),   1);
    TEST_EQUAL(d.pdf(4),   Rational(3, 216));   TEST_EQUAL(d.cdf(4),   Rational(4, 216));    TEST_EQUAL(d.ccdf(4),   Rational(215, 216));
    TEST_EQUAL(d.pdf(5),   Rational(6, 216));   TEST_EQUAL(d.cdf(5),   Rational(10, 216));   TEST_EQUAL(d.ccdf(5),   Rational(212, 216));
    TEST_EQUAL(d.pdf(6),   Rational(10, 216));  TEST_EQUAL(d.cdf(6),   Rational(20, 216));   TEST_EQUAL(d.ccdf(6),   Rational(206, 216));
    TEST_EQUAL(d.pdf(7),   Rational(15, 216));  TEST_EQUAL(d.cdf(7),   Rational(35, 216));   TEST_EQUAL(d.ccdf(7),   Rational(196, 216));
    TEST_EQUAL(d.pdf(8),   Rational(21, 216));  TEST_EQUAL(d.cdf(8),   Rational(56, 216));   TEST_EQUAL(d.ccdf(8),   Rational(181, 216));
    TEST_EQUAL(d.pdf(9),   Rational(25, 216));  TEST_EQUAL(d.cdf(9),   Rational(81, 216));   TEST_EQUAL(d.ccdf(9),   Rational(160, 216));
    TEST_EQUAL(d.pdf(10),  Rational(27, 216));  TEST_EQUAL(d.cdf(10),  Rational(108, 216));  TEST_EQUAL(d.ccdf(10),  Rational(135, 216));
    TEST_EQUAL(d.pdf(11),  Rational(27, 216));  TEST_EQUAL(d.cdf(11),  Rational(135, 216));  TEST_EQUAL(d.ccdf(11),  Rational(108, 216));
    TEST_EQUAL(d.pdf(12),  Rational(25, 216));  TEST_EQUAL(d.cdf(12),  Rational(160, 216));  TEST_EQUAL(d.ccdf(12),  Rational(81, 216));
    TEST_EQUAL(d.pdf(13),  Rational(21, 216));  TEST_EQUAL(d.cdf(13),  Rational(181, 216));  TEST_EQUAL(d.ccdf(13),  Rational(56, 216));
    TEST_EQUAL(d.pdf(14),  Rational(15, 216));  TEST_EQUAL(d.cdf(14),  Rational(196, 216));  TEST_EQUAL(d.ccdf(14),  Rational(35, 216));
    TEST_EQUAL(d.pdf(15),  Rational(10, 216));  TEST_EQUAL(d.cdf(15),  Rational(206, 216));  TEST_EQUAL(d.ccdf(15),  Rational(20, 216));
    TEST_EQUAL(d.pdf(16),  Rational(6, 216));   TEST_EQUAL(d.cdf(16),  Rational(212, 216));  TEST_EQUAL(d.ccdf(16),  Rational(10, 216));
    TEST_EQUAL(d.pdf(17),  Rational(3, 216));   TEST_EQUAL(d.cdf(17),  Rational(215, 216));  TEST_EQUAL(d.ccdf(17),  Rational(4, 216));
    TEST_EQUAL(d.pdf(18),  Rational(1, 216));   TEST_EQUAL(d.cdf(18),  1);                   TEST_EQUAL(d.ccdf(18),  Rational(1, 216));
    TEST_EQUAL(d.pdf(19),  0);                  TEST_EQUAL(d.cdf(19),  1);                   TEST_EQUAL(d.ccdf(19),  0);

    TRY(d = 2_d6 + 1_d10 - 2);

    TEST_EQUAL(d.pdf(0),   0);                  TEST_EQUAL(d.cdf(0),   0);                   TEST_EQUAL(d.ccdf(0),   1);
    TEST_EQUAL(d.pdf(1),   Rational(1, 360));   TEST_EQUAL(d.cdf(1),   Rational(1, 360));    TEST_EQUAL(d.ccdf(1),   1);
    TEST_EQUAL(d.pdf(2),   Rational(3, 360));   TEST_EQUAL(d.cdf(2),   Rational(4, 360));    TEST_EQUAL(d.ccdf(2),   Rational(359, 360));
    TEST_EQUAL(d.pdf(3),   Rational(6, 360));   TEST_EQUAL(d.cdf(3),   Rational(10, 360));   TEST_EQUAL(d.ccdf(3),   Rational(356, 360));
    TEST_EQUAL(d.pdf(4),   Rational(10, 360));  TEST_EQUAL(d.cdf(4),   Rational(20, 360));   TEST_EQUAL(d.ccdf(4),   Rational(350, 360));
    TEST_EQUAL(d.pdf(5),   Rational(15, 360));  TEST_EQUAL(d.cdf(5),   Rational(35, 360));   TEST_EQUAL(d.ccdf(5),   Rational(340, 360));
    TEST_EQUAL(d.pdf(6),   Rational(21, 360));  TEST_EQUAL(d.cdf(6),   Rational(56, 360));   TEST_EQUAL(d.ccdf(6),   Rational(325, 360));
    TEST_EQUAL(d.pdf(7),   Rational(26, 360));  TEST_EQUAL(d.cdf(7),   Rational(82, 360));   TEST_EQUAL(d.ccdf(7),   Rational(304, 360));
    TEST_EQUAL(d.pdf(8),   Rational(30, 360));  TEST_EQUAL(d.cdf(8),   Rational(112, 360));  TEST_EQUAL(d.ccdf(8),   Rational(278, 360));
    TEST_EQUAL(d.pdf(9),   Rational(33, 360));  TEST_EQUAL(d.cdf(9),   Rational(145, 360));  TEST_EQUAL(d.ccdf(9),   Rational(248, 360));
    TEST_EQUAL(d.pdf(10),  Rational(35, 360));  TEST_EQUAL(d.cdf(10),  Rational(180, 360));  TEST_EQUAL(d.ccdf(10),  Rational(215, 360));
    TEST_EQUAL(d.pdf(11),  Rational(35, 360));  TEST_EQUAL(d.cdf(11),  Rational(215, 360));  TEST_EQUAL(d.ccdf(11),  Rational(180, 360));
    TEST_EQUAL(d.pdf(12),  Rational(33, 360));  TEST_EQUAL(d.cdf(12),  Rational(248, 360));  TEST_EQUAL(d.ccdf(12),  Rational(145, 360));
    TEST_EQUAL(d.pdf(13),  Rational(30, 360));  TEST_EQUAL(d.cdf(13),  Rational(278, 360));  TEST_EQUAL(d.ccdf(13),  Rational(112, 360));
    TEST_EQUAL(d.pdf(14),  Rational(26, 360));  TEST_EQUAL(d.cdf(14),  Rational(304, 360));  TEST_EQUAL(d.ccdf(14),  Rational(82, 360));
    TEST_EQUAL(d.pdf(15),  Rational(21, 360));  TEST_EQUAL(d.cdf(15),  Rational(325, 360));  TEST_EQUAL(d.ccdf(15),  Rational(56, 360));
    TEST_EQUAL(d.pdf(16),  Rational(15, 360));  TEST_EQUAL(d.cdf(16),  Rational(340, 360));  TEST_EQUAL(d.ccdf(16),  Rational(35, 360));
    TEST_EQUAL(d.pdf(17),  Rational(10, 360));  TEST_EQUAL(d.cdf(17),  Rational(350, 360));  TEST_EQUAL(d.ccdf(17),  Rational(20, 360));
    TEST_EQUAL(d.pdf(18),  Rational(6, 360));   TEST_EQUAL(d.cdf(18),  Rational(356, 360));  TEST_EQUAL(d.ccdf(18),  Rational(10, 360));
    TEST_EQUAL(d.pdf(19),  Rational(3, 360));   TEST_EQUAL(d.cdf(19),  Rational(359, 360));  TEST_EQUAL(d.ccdf(19),  Rational(4, 360));
    TEST_EQUAL(d.pdf(20),  Rational(1, 360));   TEST_EQUAL(d.cdf(20),  1);                   TEST_EQUAL(d.ccdf(20),  Rational(1, 360));
    TEST_EQUAL(d.pdf(21),  0);                  TEST_EQUAL(d.cdf(21),  1);                   TEST_EQUAL(d.ccdf(21),  0);

}
