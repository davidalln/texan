#pragma once

#include "card.h"

card_t cards[26];

void initialize_cards(){
	cards[0] = d_newCard(ACE, SPADE);
	cards[1] = d_newCard(KING, HEART);
	cards[2] = d_newCard(QUEEN, DIAMOND);
	cards[3] = d_newCard(JACK, CLUB);
	cards[4] = d_newCard(TEN, SPADE);
	cards[5] = d_newCard(NINE, HEART);
	cards[6] = d_newCard(EIGHT, DIAMOND);
	cards[7] = d_newCard(SEVEN, CLUB);
	cards[8] = d_newCard(SIX, SPADE);
	cards[9] = d_newCard(FIVE, HEART);
	cards[10] = d_newCard(FOUR, DIAMOND);
	cards[11] = d_newCard(THREE, CLUB);
	cards[12] = d_newCard(TWO, SPADE);
	cards[13] = d_newCard(ACE, HEART);
	cards[14] = d_newCard(KING, DIAMOND);
	cards[15] = d_newCard(QUEEN, CLUB);
	cards[16] = d_newCard(JACK, SPADE);
	cards[17] = d_newCard(TEN, HEART);
	cards[18] = d_newCard(NINE, DIAMOND);
	cards[19] = d_newCard(EIGHT, CLUB);
	cards[20] = d_newCard(SEVEN, SPADE);
	cards[21] = d_newCard(SIX, HEART);
	cards[22] = d_newCard(FIVE, DIAMOND);
	cards[23] = d_newCard(FOUR, CLUB);
	cards[24] = d_newCard(THREE, SPADE);
	cards[25] = d_newCard(TWO, HEART);
}

MU_TEST(d_initialize) {
	card_t card = d_newNullCard();
	mu_check(d_isNull(card));
}

MU_TEST(d_check_valid_card) {
	card_t cards[13];

	cards[0] = d_newCard(ACE, SPADE);
	cards[1] = d_newCard(KING, HEART);
	cards[2] = d_newCard(QUEEN, DIAMOND);
	cards[3] = d_newCard(JACK, CLUB);
	cards[4] = d_newCard(TEN, SPADE);
	cards[5] = d_newCard(NINE, HEART);
	cards[6] = d_newCard(EIGHT, DIAMOND);
	cards[7] = d_newCard(SEVEN, CLUB);
	cards[8] = d_newCard(SIX, SPADE);
	cards[9] = d_newCard(FIVE, HEART);
	cards[10] = d_newCard(FOUR, DIAMOND);
	cards[11] = d_newCard(THREE, CLUB);
	cards[12] = d_newCard(TWO, SPADE);

	for (int i = 0; i < 13; i++) {
		if (i < 12)
			mu_check(cards[i].rank < cards[i + 1].rank);

		mu_check(cards[i].rank < NULL_RANK);
		mu_check(cards[i].suit < NULL_SUIT);
	}
}

MU_TEST(d_test_compare_ranks) {
	mu_assert_int_eq(1, d_compareRanks(cards[0], cards[1]));
	mu_assert_int_eq(-1, d_compareRanks(cards[3], cards[2]));
	mu_assert_int_eq(0, d_compareRanks(cards[0], cards[13]));
}

MU_TEST(d_test_compare_suits) {
	mu_assert_int_eq(1, d_compareSuits(cards[0], cards[13]));
	mu_assert_int_eq(-1, d_compareSuits(cards[14], cards[1]));
	mu_assert_int_eq(0, d_compareSuits(cards[0], cards[4]));
}

MU_TEST(d_test_compare) {
	mu_assert_int_eq(1, d_compare(cards[0], cards[1]));
	mu_assert_int_eq(-1, d_compare(cards[3], cards[2]));
	mu_assert_int_eq(0, d_compare(cards[0], cards[0]));
}

MU_TEST(d_test_encode) {
	mu_assert_int_eq(0b00000000, d_encode(cards[0]));
	mu_assert_int_eq(0b00010001, d_encode(cards[1]));
	mu_assert_int_eq(0b00100010, d_encode(cards[2]));
	mu_assert_int_eq(0b00110011, d_encode(cards[3]));
	mu_assert_int_eq(0b01000000, d_encode(cards[4]));
	mu_assert_int_eq(0b01010001, d_encode(cards[5]));
	mu_assert_int_eq(0b01100010, d_encode(cards[6]));
	mu_assert_int_eq(0b01110011, d_encode(cards[7]));
	mu_assert_int_eq(0b10000000, d_encode(cards[8]));
	mu_assert_int_eq(0b10010001, d_encode(cards[9]));
	mu_assert_int_eq(0b10100010, d_encode(cards[10]));
	mu_assert_int_eq(0b10110011, d_encode(cards[11]));
	mu_assert_int_eq(0b11000000, d_encode(cards[12]));
}

MU_TEST(d_test_toString) {
	char str[3];

	d_toString(cards[0], str);
	mu_assert_string_eq("As", str);
	d_toString(cards[1], str);
	mu_assert_string_eq("Kh", str);
	d_toString(cards[2], str);
	mu_assert_string_eq("Qd", str);
	d_toString(cards[3], str);
	mu_assert_string_eq("Jc", str);
	d_toString(cards[4], str);
	mu_assert_string_eq("Ts", str);
	d_toString(cards[5], str);
	mu_assert_string_eq("9h", str);
	d_toString(cards[6], str);
	mu_assert_string_eq("8d", str);
	d_toString(cards[7], str);
	mu_assert_string_eq("7c", str);
	d_toString(cards[8], str);
	mu_assert_string_eq("6s", str);
	d_toString(cards[9], str);
	mu_assert_string_eq("5h", str);
	d_toString(cards[10], str);
	mu_assert_string_eq("4d", str);
	d_toString(cards[11], str);
	mu_assert_string_eq("3c", str);
	d_toString(cards[12], str);
	mu_assert_string_eq("2s", str);
}

MU_TEST_SUITE(card_test_suite) {
	MU_RUN_TEST(d_initialize);
	MU_RUN_TEST(d_check_valid_card);
	MU_RUN_TEST(d_test_compare_ranks);
	MU_RUN_TEST(d_test_compare_suits);
	MU_RUN_TEST(d_test_compare);
	MU_RUN_TEST(d_test_encode);
	MU_RUN_TEST(d_test_toString);
}

