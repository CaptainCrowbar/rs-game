// This file is generated by the rs-update-tests script

#include "rs-unit-test.hpp"

int main(int argc, char** argv) {

    RS::UnitTest::begin_tests(argc, argv);

    // dice-test.cpp
    UNIT_TEST(rs_game_dice_arithmetic)
    UNIT_TEST(rs_game_dice_statistics)
    UNIT_TEST(rs_game_dice_parser)
    UNIT_TEST(rs_game_dice_generation)
    UNIT_TEST(rs_game_dice_literals)
    UNIT_TEST(rs_game_dice_pdf)

    // english-test.cpp
    UNIT_TEST(rs_game_english_case_conversion)
    UNIT_TEST(rs_game_english_list_formatting)
    UNIT_TEST(rs_game_english_number_formatting)
    UNIT_TEST(rs_game_english_pluralization)
    UNIT_TEST(rs_game_english_lorem_ipsum)

    // hexmap-geometry-test.cpp
    UNIT_TEST(rs_game_hexmap_coordinate_operations)

    // hexmap-building-test.cpp
    UNIT_TEST(rs_game_hexmap_adjacency_properties)
    UNIT_TEST(rs_game_hexmap_initializer_list)

    // hexmap-art-test.cpp
    UNIT_TEST(rs_game_hexmap_ascii_art)
    UNIT_TEST(rs_game_hexmap_emoji_art)
    UNIT_TEST(rs_game_hexmap_xterm_art)

    // markov-test.cpp
    UNIT_TEST(rs_game_markov_character_mode)
    UNIT_TEST(rs_game_markov_string_mode)

    // text-gen-test.cpp
    UNIT_TEST(rs_game_text_generation_null)
    UNIT_TEST(rs_game_text_generation_constant)
    UNIT_TEST(rs_game_text_generation_number)
    UNIT_TEST(rs_game_text_generation_select)
    UNIT_TEST(rs_game_text_generation_weighted)
    UNIT_TEST(rs_game_text_generation_sequence)
    UNIT_TEST(rs_game_text_generation_optional)
    UNIT_TEST(rs_game_text_generation_repeat)
    UNIT_TEST(rs_game_text_generation_transform)
    UNIT_TEST(rs_game_text_generation_literals)

    // version-test.cpp
    UNIT_TEST(rs_game_version)

    // unit-test.cpp

    return RS::UnitTest::end_tests();

}
