#pragma once

#include "hand.h"

hand_t hands[270];

void initialize_hands() {
	int index = 0;

	// A2s+
	for (int i = 1; i < 13; i++) {
		hands[index++] = h_newHand(d_newCard(ACE, SPADE), d_newCard(i, SPADE));
		hands[index++] = h_newHand(d_newCard(ACE, HEART), d_newCard(i, HEART));
		hands[index++] = h_newHand(d_newCard(ACE, DIAMOND), d_newCard(i, DIAMOND));
		hands[index++] = h_newHand(d_newCard(ACE, CLUB), d_newCard(i, CLUB));
	}

	// A2o+
	for (int i = 1; i < 13; i++) {
		hands[index++] = h_newHand(d_newCard(ACE, SPADE), d_newCard(i, HEART));
		hands[index++] = h_newHand(d_newCard(ACE, SPADE), d_newCard(i, DIAMOND));
		hands[index++] = h_newHand(d_newCard(ACE, SPADE), d_newCard(i, CLUB));
		hands[index++] = h_newHand(d_newCard(ACE, HEART), d_newCard(i, SPADE));
		hands[index++] = h_newHand(d_newCard(ACE, HEART), d_newCard(i, DIAMOND));
		hands[index++] = h_newHand(d_newCard(ACE, HEART), d_newCard(i, CLUB));
		hands[index++] = h_newHand(d_newCard(ACE, DIAMOND), d_newCard(i, SPADE));
		hands[index++] = h_newHand(d_newCard(ACE, DIAMOND), d_newCard(i, HEART));
		hands[index++] = h_newHand(d_newCard(ACE, DIAMOND), d_newCard(i, CLUB));
		hands[index++] = h_newHand(d_newCard(ACE, CLUB), d_newCard(i, SPADE));
		hands[index++] = h_newHand(d_newCard(ACE, CLUB), d_newCard(i, HEART));
		hands[index++] = h_newHand(d_newCard(ACE, CLUB), d_newCard(i, DIAMOND));
	}

	// 22+
	for (int i = 0; i < 13; i++) {
		hands[index++] = h_newHand(d_newCard(i, SPADE), d_newCard(i, HEART));
		hands[index++] = h_newHand(d_newCard(i, SPADE), d_newCard(i, DIAMOND));
		hands[index++] = h_newHand(d_newCard(i, SPADE), d_newCard(i, CLUB));
		hands[index++] = h_newHand(d_newCard(i, HEART), d_newCard(i, DIAMOND));
		hands[index++] = h_newHand(d_newCard(i, HEART), d_newCard(i, CLUB));
		hands[index++] = h_newHand(d_newCard(i, DIAMOND), d_newCard(i, CLUB));
	}
}

#define assert_hand_eq(a, b) do {\
	mu_assert_int_eq(a.cards[0].rank, b.cards[0].rank);\
	mu_assert_int_eq(a.cards[1].rank, b.cards[1].rank);\
	mu_assert_int_eq(a.cards[0].suit, b.cards[0].suit);\
	mu_assert_int_eq(a.cards[1].suit, b.cards[1].suit);\
} while (0)

MU_TEST(hand_test_null) {
	hand_t nullHand = h_newNullHand();
	mu_check(h_isNull(nullHand));
}

MU_TEST(hand_test_not_null) {
	for (int i = 0; i < 270; i++) {
		hand_t hand = hands[i];
		mu_check(!h_isNull(hand));
	}
}

MU_TEST(hand_invalid) {
	hand_t hand = h_newHand(d_newCard(ACE, SPADE), d_newCard(ACE, SPADE));
	mu_check(h_isNull(hand));
}

MU_TEST(hand_sorted_correctly) {
	for (int i = 0; i < 270; i++) {
		hand_t hand = hands[i];
		hand_t reverse = h_newHand(hand.cards[1], hand.cards[0]);
		mu_check(reverse.cards[0].rank <= reverse.cards[1].rank);
		if (reverse.cards[0].rank == reverse.cards[1].rank) {
			mu_check(reverse.cards[0].suit < reverse.cards[1].suit);
		}
	}
}

MU_TEST(hand_test_toString) {
	char str[5];
	
	h_toString(hands[0], str);
	mu_assert_string_eq("AsKs", str);	
	h_toString(hands[1], str);
	mu_assert_string_eq("AhKh", str);	
	h_toString(hands[2], str);
	mu_assert_string_eq("AdKd", str);	
	h_toString(hands[3], str);
	mu_assert_string_eq("AcKc", str);	
	h_toString(hands[4], str);
	mu_assert_string_eq("AsQs", str);	
	h_toString(hands[8], str);
	mu_assert_string_eq("AsJs", str);	
	h_toString(hands[12], str);
	mu_assert_string_eq("AsTs", str);	
	h_toString(hands[16], str);
	mu_assert_string_eq("As9s", str);	
	h_toString(hands[20], str);
	mu_assert_string_eq("As8s", str);	
	h_toString(hands[24], str);
	mu_assert_string_eq("As7s", str);	
	h_toString(hands[28], str);
	mu_assert_string_eq("As6s", str);	
	h_toString(hands[32], str);
	mu_assert_string_eq("As5s", str);	
	h_toString(hands[36], str);
	mu_assert_string_eq("As4s", str);	
	h_toString(hands[40], str);
	mu_assert_string_eq("As3s", str);	
	h_toString(hands[44], str);
	mu_assert_string_eq("As2s", str);	

	h_toString(hands[48], str);
	mu_assert_string_eq("AsKh", str);	
	h_toString(hands[49], str);
	mu_assert_string_eq("AsKd", str);	
	h_toString(hands[50], str);
	mu_assert_string_eq("AsKc", str);	
	h_toString(hands[51], str);
	mu_assert_string_eq("AhKs", str);	
	h_toString(hands[52], str);
	mu_assert_string_eq("AhKd", str);	
	h_toString(hands[53], str);
	mu_assert_string_eq("AhKc", str);	
	h_toString(hands[54], str);
	mu_assert_string_eq("AdKs", str);	
	h_toString(hands[55], str);
	mu_assert_string_eq("AdKh", str);	
	h_toString(hands[56], str);
	mu_assert_string_eq("AdKc", str);	
	h_toString(hands[57], str);
	mu_assert_string_eq("AcKs", str);	
	h_toString(hands[58], str);
	mu_assert_string_eq("AcKh", str);	
	h_toString(hands[59], str);
	mu_assert_string_eq("AcKd", str);	

	h_toString(hands[192], str);
	mu_assert_string_eq("AsAh", str);
	h_toString(hands[193], str);
	mu_assert_string_eq("AsAd", str);
	h_toString(hands[194], str);
	mu_assert_string_eq("AsAc", str);
	h_toString(hands[195], str);
	mu_assert_string_eq("AhAd", str);
	h_toString(hands[196], str);
	mu_assert_string_eq("AhAc", str);
	h_toString(hands[197], str);
	mu_assert_string_eq("AdAc", str);
	h_toString(hands[198], str);
	mu_assert_string_eq("KsKh", str);
	h_toString(hands[204], str);
	mu_assert_string_eq("QsQh", str);
	h_toString(hands[210], str);
	mu_assert_string_eq("JsJh", str);
	h_toString(hands[216], str);
	mu_assert_string_eq("TsTh", str);
	h_toString(hands[222], str);
	mu_assert_string_eq("9s9h", str);
	h_toString(hands[228], str);
	mu_assert_string_eq("8s8h", str);
	h_toString(hands[234], str);
	mu_assert_string_eq("7s7h", str);
	h_toString(hands[240], str);
	mu_assert_string_eq("6s6h", str);
	h_toString(hands[246], str);
	mu_assert_string_eq("5s5h", str);
	h_toString(hands[252], str);
	mu_assert_string_eq("4s4h", str);
	h_toString(hands[258], str);
	mu_assert_string_eq("3s3h", str);
	h_toString(hands[264], str);
	mu_assert_string_eq("2s2h", str);
}

MU_TEST(hand_test_compare) {
	hand_t AsKs = hands[0];
	assert_hand_eq(AsKs, h_newHand(d_newCard(ACE, SPADE), d_newCard(KING, SPADE)));
	for (int i = 1; i < 12 * 16; i++) {
		hand_t compare = hands[i];
		mu_assert_int_eq(1, h_compare(AsKs, compare));
	}

	hand_t Ac2c = hands[47];
	assert_hand_eq(Ac2c, h_newHand(d_newCard(ACE, CLUB), d_newCard(TWO, CLUB)));
	for (int i = 0; i < 12 * 16; i++) {
		hand_t compare = hands[i];

		if (i != 47) {
			mu_assert_int_eq(-1, h_compare(Ac2c, compare));
		} else {
			mu_assert_int_eq(0, h_compare(Ac2c, compare));
		}
	}

	hand_t AsAh = hands[192];
	assert_hand_eq(AsAh, h_newHand(d_newCard(ACE, SPADE), d_newCard(ACE, HEART)));
	for (int i = 193; i < 270; i++) {
		hand_t compare = hands[i];
		mu_assert_int_eq(1, h_compare(AsAh, compare));
	}

	hand_t _2d2c = hands[269];
	for (int i = 192; i < 269; i++) {
		mu_assert_int_eq(-1, h_compare(_2d2c, hands[i]));
	}

	mu_assert_int_eq(0, h_compare(_2d2c, hands[269]));
}

MU_TEST(hand_encode) {
	hand_t hand = h_newHand(d_newCard(ACE, SPADE), d_newCard(ACE, HEART));
	data encode = h_encode(hand);
	mu_assert_int_eq(1, encode);
}

MU_TEST(hand_decode) {
	hand_t hand;
	h_decode(1, &hand);
	mu_check(!h_isNull(hand));
	mu_assert_int_eq(ACE, hand.cards[0].rank);
	mu_assert_int_eq(SPADE, hand.cards[0].suit);
	mu_assert_int_eq(ACE, hand.cards[1].rank);
	mu_assert_int_eq(HEART, hand.cards[1].suit);
}

MU_TEST(hand_decode_encoded) {
	for (int i = 0; i < 270; i++) {
		hand_t proof = hands[i];
		hand_t transformed;
		
		h_decode(h_encode(proof), &transformed);
		
		assert_hand_eq(proof, transformed);
	}
}

MU_TEST(hand_calc_strength) {
	hand_t hand0 = h_newHand(d_newCard(ACE, SPADE), d_newCard(ACE, HEART));
	hand_t hand1 = h_newHand(d_newCard(ACE, SPADE), d_newCard(TWO, DIAMOND));
	hand_t hand2 = h_newHand(d_newCard(TWO, SPADE), d_newCard(TWO, DIAMOND));
	hand_t hand3 = h_newHand(d_newCard(THREE, HEART), d_newCard(THREE, SPADE));
	hand_t hand4 = h_newHand(d_newCard(ACE, CLUB), d_newCard(KING, DIAMOND));
	hand_t hand5 = h_newHand(d_newCard(ACE, SPADE), d_newCard(KING, SPADE));
	hand_t hand6 = h_newHand(d_newCard(NINE, CLUB), d_newCard(EIGHT, SPADE));

	card_t community0[3] = { d_newCard(TWO, CLUB), d_newCard(THREE, DIAMOND), d_newCard(FOUR, CLUB) };
	card_t community1[3] = { d_newCard(TWO, CLUB), d_newCard(THREE, DIAMOND), d_newCard(THREE, CLUB) };
	card_t community2[3] = { d_newCard(QUEEN, SPADE), d_newCard(JACK, SPADE), d_newCard(TEN, SPADE) };
	card_t community3[5] = { d_newCard(QUEEN, SPADE), d_newCard(JACK, SPADE), d_newCard(TEN, SPADE), d_newCard(NINE, SPADE), d_newCard(TWO, CLUB) };

	hand0 = h_calcStrength(hand0, community0, 3);
	hand1 = h_calcStrength(hand1, community0, 3);
	hand2 = h_calcStrength(hand2, community0, 3);
	hand3 = h_calcStrength(hand3, community0, 3);
	hand4 = h_calcStrength(hand4, community0, 3);
	mu_assert_int_eq(ONE_PAIR, h_strength(hand0));
	mu_assert_int_eq(ONE_PAIR, h_strength(hand1));
	mu_assert_int_eq(THREE_OF_A_KIND, h_strength(hand2));
	mu_assert_int_eq(THREE_OF_A_KIND, h_strength(hand3));
	mu_assert_int_eq(HIGH_CARD, h_strength(hand4));

	mu_check(hand0.strength < hand1.strength);
	mu_check(hand0.strength > hand2.strength);
	mu_check(hand0.strength > hand3.strength);
	mu_check(hand0.strength < hand4.strength);
	mu_check(hand1.strength > hand2.strength);
	mu_check(hand1.strength > hand3.strength);
	mu_check(hand1.strength < hand4.strength);
	mu_check(hand2.strength > hand3.strength);
	mu_check(hand2.strength < hand4.strength);
	mu_check(hand3.strength < hand4.strength);

	hand0 = h_calcStrength(hand0, community1, 3);
	hand1 = h_calcStrength(hand1, community1, 3);
	hand2 = h_calcStrength(hand2, community1, 3);
	hand3 = h_calcStrength(hand3, community1, 3);
	hand4 = h_calcStrength(hand4, community1, 3);
	mu_assert_int_eq(TWO_PAIR, h_strength(hand0));
	mu_assert_int_eq(TWO_PAIR, h_strength(hand1));
	mu_assert_int_eq(FULL_HOUSE, h_strength(hand2));
	mu_assert_int_eq(FOUR_OF_A_KIND, h_strength(hand3));
	mu_assert_int_eq(ONE_PAIR, h_strength(hand4));

	mu_check(hand0.strength < hand1.strength);
	mu_check(hand0.strength > hand2.strength);
	mu_check(hand0.strength > hand3.strength);
	mu_check(hand0.strength < hand4.strength);
	mu_check(hand1.strength > hand2.strength);
	mu_check(hand1.strength > hand3.strength);
	mu_check(hand1.strength < hand4.strength);
	mu_check(hand2.strength > hand3.strength);
	mu_check(hand2.strength < hand4.strength);
	mu_check(hand3.strength < hand4.strength);

	hand0 = h_calcStrength(hand0, community2, 3);
	hand1 = h_calcStrength(hand1, community2, 3);
	hand2 = h_calcStrength(hand2, community2, 3);
	hand3 = h_calcStrength(hand3, community2, 3);
	hand4 = h_calcStrength(hand4, community2, 3);
	hand5 = h_calcStrength(hand5, community2, 3);
	mu_assert_int_eq(ONE_PAIR, h_strength(hand0));
	mu_assert_int_eq(HIGH_CARD, h_strength(hand1));
	mu_assert_int_eq(ONE_PAIR, h_strength(hand2));
	mu_assert_int_eq(ONE_PAIR, h_strength(hand3));
	mu_assert_int_eq(STRAIGHT, h_strength(hand4));
	mu_assert_int_eq(STRAIGHT_FLUSH, h_strength(hand5));

	mu_check(hand0.strength < hand1.strength);
	mu_check(hand0.strength < hand2.strength);
	mu_check(hand0.strength < hand3.strength);
	mu_check(hand0.strength > hand4.strength);
	mu_check(hand0.strength > hand5.strength);
	mu_check(hand1.strength > hand2.strength);
	mu_check(hand1.strength > hand3.strength);
	mu_check(hand1.strength > hand4.strength);
	mu_check(hand1.strength > hand5.strength);
	mu_check(hand2.strength > hand3.strength);
	mu_check(hand2.strength > hand4.strength);
	mu_check(hand2.strength > hand5.strength);
	mu_check(hand3.strength > hand4.strength);
	mu_check(hand3.strength > hand5.strength);
	mu_check(hand4.strength > hand5.strength);

	hand0 = h_calcStrength(hand0, community3, 5);
	hand1 = h_calcStrength(hand1, community3, 5);
	hand2 = h_calcStrength(hand2, community3, 5);
	hand3 = h_calcStrength(hand3, community3, 5);
	hand4 = h_calcStrength(hand4, community3, 5);
	hand5 = h_calcStrength(hand5, community3, 5);
	hand6 = h_calcStrength(hand6, community3, 5);
	mu_assert_int_eq(FLUSH, h_strength(hand0));
	mu_assert_int_eq(FLUSH, h_strength(hand1));
	mu_assert_int_eq(FLUSH, h_strength(hand2));
	mu_assert_int_eq(FLUSH, h_strength(hand3));
	mu_assert_int_eq(STRAIGHT, h_strength(hand4));
	mu_assert_int_eq(STRAIGHT_FLUSH, h_strength(hand5));
	mu_assert_int_eq(STRAIGHT_FLUSH, h_strength(hand6));

	mu_check(hand0.strength == hand1.strength);
	mu_check(hand0.strength < hand2.strength);
	mu_check(hand0.strength < hand3.strength);
	mu_check(hand0.strength < hand4.strength);
	mu_check(hand0.strength > hand5.strength);
	mu_check(hand0.strength > hand6.strength);
	mu_check(hand1.strength < hand2.strength);
	mu_check(hand1.strength < hand3.strength);
	mu_check(hand1.strength < hand4.strength);
	mu_check(hand1.strength > hand5.strength);
	mu_check(hand1.strength > hand6.strength);
	mu_check(hand2.strength > hand3.strength);
	mu_check(hand2.strength < hand4.strength);
	mu_check(hand2.strength > hand5.strength);
	mu_check(hand2.strength > hand6.strength);
	mu_check(hand3.strength < hand4.strength);
	mu_check(hand3.strength > hand5.strength);
	mu_check(hand3.strength > hand6.strength);
	mu_check(hand4.strength > hand5.strength);
	mu_check(hand4.strength > hand6.strength);
	mu_check(hand5.strength < hand6.strength);
}

MU_TEST(hand_calc_strength_edge0) {
	hand_t hand0 = h_newHand(d_newCard(ACE, SPADE), d_newCard(ACE, HEART));
	hand_t hand1 = h_newHand(d_newCard(ACE, DIAMOND), d_newCard(FIVE, DIAMOND));
	hand_t hand2 = h_newHand(d_newCard(ACE, CLUB), d_newCard(FIVE, CLUB));
	
	card_t community[5];
	community[0] = d_newCard(KING, HEART);
	community[1] = d_newCard(SEVEN, SPADE);
	community[2] = d_newCard(TWO, CLUB);
	community[3] = d_newCard(THREE, HEART);
	community[4] = d_newCard(FOUR, CLUB);

	hand0 = h_calcStrength(hand0, community, 5);
	hand1 = h_calcStrength(hand1, community, 5);
	hand2 = h_calcStrength(hand2, community, 5);

	mu_assert_int_eq(ONE_PAIR, h_strength(hand0));
	mu_assert_int_eq(STRAIGHT, h_strength(hand1));
	mu_assert_int_eq(STRAIGHT, h_strength(hand2));
}

MU_TEST_SUITE(hand_test_suite) {
	MU_RUN_TEST(hand_test_null);
	MU_RUN_TEST(hand_test_not_null);
	MU_RUN_TEST(hand_invalid);
	MU_RUN_TEST(hand_sorted_correctly);
	MU_RUN_TEST(hand_test_toString);
	MU_RUN_TEST(hand_test_compare);
	MU_RUN_TEST(hand_encode);
	MU_RUN_TEST(hand_decode);
	MU_RUN_TEST(hand_decode_encoded);
	MU_RUN_TEST(hand_calc_strength);
	MU_RUN_TEST(hand_calc_strength_edge0);
}

