#pragma once

#include "combo.h"

combo_t combos[169];

void initialize_combos() {
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++) {
			combos[i * 13 + j] = c_newCombo(i, j);
		}
	}
}

void delete_combos() {
	for (int i = 0; i < 169; i++) {
		c_deleteCombo(combos[i]);
	}
}

#define assert_combo_eq(a, b) do {\
	mu_assert_int_eq(a.ranks[0], b.ranks[0]);\
	mu_assert_int_eq(a.ranks[1], b.ranks[1]);\
	mu_assert_int_eq(a.type, b.type);\
	mu_assert_int_eq(a.hands.length, b.hands.length);\
	mu_assert_int_eq(0, c_compare(a, b));\
} while (0)

MU_TEST(combo_null) {
	combo_t combo = c_newNullCombo();
	mu_check(c_isNull(combo));
	c_deleteCombo(combo);
}

MU_TEST(combo_initialize_pair) {
	for (int i = 0; i < 13; i++) {
		combo_t comboProof = combos[i * 13 + i];

		combo_t combo = c_newCombo(i, i);
		mu_check(!c_isNull(combo));


		assert_combo_eq(comboProof, combo);
		mu_assert_int_eq(PAIR, combo.type);
		mu_assert_int_eq(6, combo.hands.length);

		combo.hands = ll_head(combo.hands);
		comboProof.hands = ll_head(comboProof.hands);
		for (int x = 0; x < 6; x++) {
			hand_t hand;
			hand_t handProof;
			ll_get(combo.hands, &hand);	
			ll_get(comboProof.hands, &handProof);

			assert_hand_eq(handProof, hand);

			combo.hands = ll_next(combo.hands);
			comboProof.hands = ll_next(comboProof.hands);
		}

		c_deleteCombo(combo);

		combo_t blankCombo = c_newBlankCombo(i, i, PAIR);
		mu_check(blankCombo.type == PAIR);
		mu_check(c_isNull(blankCombo));
		c_deleteCombo(blankCombo);
	}
}

MU_TEST(combo_initialize_suited) {
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++) {
			if (i < j) {
				combo_t comboProof = combos[i * 13 + j];

				combo_t combo = c_newCombo(i, j);
				mu_check(!c_isNull(combo));

				assert_combo_eq(comboProof, combo);
				mu_assert_int_eq(SUITED, combo.type);
				mu_assert_int_eq(4, combo.hands.length);

				combo.hands = ll_head(combo.hands);
				comboProof.hands = ll_head(comboProof.hands);
				for (int x = 0; x < 4; x++) {
					hand_t hand;
					hand_t handProof;
					ll_get(combo.hands, &hand);	
					ll_get(comboProof.hands, &handProof);

					assert_hand_eq(handProof, hand);

					combo.hands = ll_next(combo.hands);
					comboProof.hands = ll_next(comboProof.hands);
				}

				c_deleteCombo(combo);

				combo_t blankCombo = c_newBlankCombo(i, j, SUITED);
				mu_check(blankCombo.type == SUITED);
				mu_check(c_isNull(blankCombo));
				c_deleteCombo(blankCombo);
			}
		}
	}
}

MU_TEST(combo_initialize_offsuit) {
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++) {
			if (i > j) {
				combo_t comboProof = combos[i * 13 + j];

				combo_t combo = c_newCombo(i, j);
				mu_check(!c_isNull(combo));

				assert_combo_eq(comboProof, combo);
				mu_assert_int_eq(OFFSUIT, combo.type);
				mu_assert_int_eq(12, combo.hands.length);

				combo.hands = ll_head(combo.hands);
				comboProof.hands = ll_head(comboProof.hands);
				for (int x = 0; x < 12; x++) {
					hand_t hand;
					hand_t handProof;
					ll_get(combo.hands, &hand);	
					ll_get(comboProof.hands, &handProof);

					assert_hand_eq(handProof, hand);

					combo.hands = ll_next(combo.hands);
					comboProof.hands = ll_next(comboProof.hands);
				}

				c_deleteCombo(combo);

				combo_t blankCombo = c_newBlankCombo(i, j, OFFSUIT);
				mu_check(blankCombo.type == OFFSUIT);
				mu_check(c_isNull(blankCombo));
				c_deleteCombo(blankCombo);
			}
		}
	}
}

MU_TEST(combo_test_toString) {
	char str[4];

	c_toString(combos[0], str);
	mu_assert_string_eq("AA", str);
	c_toString(combos[1], str);
	mu_assert_string_eq("AKs", str);
	c_toString(combos[2], str);
	mu_assert_string_eq("AQs", str);
	c_toString(combos[3], str);
	mu_assert_string_eq("AJs", str);
	c_toString(combos[4], str);
	mu_assert_string_eq("ATs", str);
	c_toString(combos[5], str);
	mu_assert_string_eq("A9s", str);
	c_toString(combos[6], str);
	mu_assert_string_eq("A8s", str);
	c_toString(combos[7], str);
	mu_assert_string_eq("A7s", str);
	c_toString(combos[8], str);
	mu_assert_string_eq("A6s", str);
	c_toString(combos[9], str);
	mu_assert_string_eq("A5s", str);
	c_toString(combos[10], str);
	mu_assert_string_eq("A4s", str);
	c_toString(combos[11], str);
	mu_assert_string_eq("A3s", str);
	c_toString(combos[12], str);
	mu_assert_string_eq("A2s", str);

	c_toString(combos[13 + 0], str);
	mu_assert_string_eq("AKo", str);
	c_toString(combos[13 + 1], str);
	mu_assert_string_eq("KK", str);
	c_toString(combos[13 + 2], str);
	mu_assert_string_eq("KQs", str);
	c_toString(combos[13 + 3], str);
	mu_assert_string_eq("KJs", str);
	c_toString(combos[13 + 4], str);
	mu_assert_string_eq("KTs", str);
	c_toString(combos[13 + 5], str);
	mu_assert_string_eq("K9s", str);
	c_toString(combos[13 + 6], str);
	mu_assert_string_eq("K8s", str);
	c_toString(combos[13 + 7], str);
	mu_assert_string_eq("K7s", str);
	c_toString(combos[13 + 8], str);
	mu_assert_string_eq("K6s", str);
	c_toString(combos[13 + 9], str);
	mu_assert_string_eq("K5s", str);
	c_toString(combos[13 + 10], str);
	mu_assert_string_eq("K4s", str);
	c_toString(combos[13 + 11], str);
	mu_assert_string_eq("K3s", str);
	c_toString(combos[13 + 12], str);
	mu_assert_string_eq("K2s", str);

	c_toString(combos[13 * 2 + 0], str);
	mu_assert_string_eq("AQo", str);
	c_toString(combos[13 * 2 + 1], str);
	mu_assert_string_eq("KQo", str);
	c_toString(combos[13 * 2 + 2], str);
	mu_assert_string_eq("QQ", str);
	c_toString(combos[13 * 2 + 3], str);
	mu_assert_string_eq("QJs", str);
	c_toString(combos[13 * 2 + 4], str);
	mu_assert_string_eq("QTs", str);
	c_toString(combos[13 * 2 + 5], str);
	mu_assert_string_eq("Q9s", str);
	c_toString(combos[13 * 2 + 6], str);
	mu_assert_string_eq("Q8s", str);
	c_toString(combos[13 * 2 + 7], str);
	mu_assert_string_eq("Q7s", str);
	c_toString(combos[13 * 2 + 8], str);
	mu_assert_string_eq("Q6s", str);
	c_toString(combos[13 * 2 + 9], str);
	mu_assert_string_eq("Q5s", str);
	c_toString(combos[13 * 2 + 10], str);
	mu_assert_string_eq("Q4s", str);
	c_toString(combos[13 * 2 + 11], str);
	mu_assert_string_eq("Q3s", str);
	c_toString(combos[13 * 2 + 12], str);
	mu_assert_string_eq("Q2s", str);

	c_toString(combos[13 * 3 + 0], str);
	mu_assert_string_eq("AJo", str);
	c_toString(combos[13 * 3 + 1], str);
	mu_assert_string_eq("KJo", str);
	c_toString(combos[13 * 3 + 2], str);
	mu_assert_string_eq("QJo", str);
	c_toString(combos[13 * 3 + 3], str);
	mu_assert_string_eq("JJ", str);
	c_toString(combos[13 * 3 + 4], str);
	mu_assert_string_eq("JTs", str);
	c_toString(combos[13 * 3 + 5], str);
	mu_assert_string_eq("J9s", str);
	c_toString(combos[13 * 3 + 6], str);
	mu_assert_string_eq("J8s", str);
	c_toString(combos[13 * 3 + 7], str);
	mu_assert_string_eq("J7s", str);
	c_toString(combos[13 * 3 + 8], str);
	mu_assert_string_eq("J6s", str);
	c_toString(combos[13 * 3 + 9], str);
	mu_assert_string_eq("J5s", str);
	c_toString(combos[13 * 3 + 10], str);
	mu_assert_string_eq("J4s", str);
	c_toString(combos[13 * 3 + 11], str);
	mu_assert_string_eq("J3s", str);
	c_toString(combos[13 * 3 + 12], str);
	mu_assert_string_eq("J2s", str);

	c_toString(combos[13 * 4 + 0], str);
	mu_assert_string_eq("ATo", str);
	c_toString(combos[13 * 4 + 1], str);
	mu_assert_string_eq("KTo", str);
	c_toString(combos[13 * 4 + 2], str);
	mu_assert_string_eq("QTo", str);
	c_toString(combos[13 * 4 + 3], str);
	mu_assert_string_eq("JTo", str);
	c_toString(combos[13 * 4 + 4], str);
	mu_assert_string_eq("TT", str);
	c_toString(combos[13 * 4 + 5], str);
	mu_assert_string_eq("T9s", str);
	c_toString(combos[13 * 4 + 6], str);
	mu_assert_string_eq("T8s", str);
	c_toString(combos[13 * 4 + 7], str);
	mu_assert_string_eq("T7s", str);
	c_toString(combos[13 * 4 + 8], str);
	mu_assert_string_eq("T6s", str);
	c_toString(combos[13 * 4 + 9], str);
	mu_assert_string_eq("T5s", str);
	c_toString(combos[13 * 4 + 10], str);
	mu_assert_string_eq("T4s", str);
	c_toString(combos[13 * 4 + 11], str);
	mu_assert_string_eq("T3s", str);
	c_toString(combos[13 * 4 + 12], str);
	mu_assert_string_eq("T2s", str);

	c_toString(combos[13 * 5 + 0], str);
	mu_assert_string_eq("A9o", str);
	c_toString(combos[13 * 5 + 1], str);
	mu_assert_string_eq("K9o", str);
	c_toString(combos[13 * 5 + 2], str);
	mu_assert_string_eq("Q9o", str);
	c_toString(combos[13 * 5 + 3], str);
	mu_assert_string_eq("J9o", str);
	c_toString(combos[13 * 5 + 4], str);
	mu_assert_string_eq("T9o", str);
	c_toString(combos[13 * 5 + 5], str);
	mu_assert_string_eq("99", str);
	c_toString(combos[13 * 5 + 6], str);
	mu_assert_string_eq("98s", str);
	c_toString(combos[13 * 5 + 7], str);
	mu_assert_string_eq("97s", str);
	c_toString(combos[13 * 5 + 8], str);
	mu_assert_string_eq("96s", str);
	c_toString(combos[13 * 5 + 9], str);
	mu_assert_string_eq("95s", str);
	c_toString(combos[13 * 5 + 10], str);
	mu_assert_string_eq("94s", str);
	c_toString(combos[13 * 5 + 11], str);
	mu_assert_string_eq("93s", str);
	c_toString(combos[13 * 5 + 12], str);
	mu_assert_string_eq("92s", str);

	c_toString(combos[13 * 6 + 0], str);
	mu_assert_string_eq("A8o", str);
	c_toString(combos[13 * 6 + 1], str);
	mu_assert_string_eq("K8o", str);
	c_toString(combos[13 * 6 + 2], str);
	mu_assert_string_eq("Q8o", str);
	c_toString(combos[13 * 6 + 3], str);
	mu_assert_string_eq("J8o", str);
	c_toString(combos[13 * 6 + 4], str);
	mu_assert_string_eq("T8o", str);
	c_toString(combos[13 * 6 + 5], str);
	mu_assert_string_eq("98o", str);
	c_toString(combos[13 * 6 + 6], str);
	mu_assert_string_eq("88", str);
	c_toString(combos[13 * 6 + 7], str);
	mu_assert_string_eq("87s", str);
	c_toString(combos[13 * 6 + 8], str);
	mu_assert_string_eq("86s", str);
	c_toString(combos[13 * 6 + 9], str);
	mu_assert_string_eq("85s", str);
	c_toString(combos[13 * 6 + 10], str);
	mu_assert_string_eq("84s", str);
	c_toString(combos[13 * 6 + 11], str);
	mu_assert_string_eq("83s", str);
	c_toString(combos[13 * 6 + 12], str);
	mu_assert_string_eq("82s", str);

	c_toString(combos[13 * 7 + 0], str);
	mu_assert_string_eq("A7o", str);
	c_toString(combos[13 * 7 + 1], str);
	mu_assert_string_eq("K7o", str);
	c_toString(combos[13 * 7 + 2], str);
	mu_assert_string_eq("Q7o", str);
	c_toString(combos[13 * 7 + 3], str);
	mu_assert_string_eq("J7o", str);
	c_toString(combos[13 * 7 + 4], str);
	mu_assert_string_eq("T7o", str);
	c_toString(combos[13 * 7 + 5], str);
	mu_assert_string_eq("97o", str);
	c_toString(combos[13 * 7 + 6], str);
	mu_assert_string_eq("87o", str);
	c_toString(combos[13 * 7 + 7], str);
	mu_assert_string_eq("77", str);
	c_toString(combos[13 * 7 + 8], str);
	mu_assert_string_eq("76s", str);
	c_toString(combos[13 * 7 + 9], str);
	mu_assert_string_eq("75s", str);
	c_toString(combos[13 * 7 + 10], str);
	mu_assert_string_eq("74s", str);
	c_toString(combos[13 * 7 + 11], str);
	mu_assert_string_eq("73s", str);
	c_toString(combos[13 * 7 + 12], str);
	mu_assert_string_eq("72s", str);

	c_toString(combos[13 * 8 + 0], str);
	mu_assert_string_eq("A6o", str);
	c_toString(combos[13 * 8 + 1], str);
	mu_assert_string_eq("K6o", str);
	c_toString(combos[13 * 8 + 2], str);
	mu_assert_string_eq("Q6o", str);
	c_toString(combos[13 * 8 + 3], str);
	mu_assert_string_eq("J6o", str);
	c_toString(combos[13 * 8 + 4], str);
	mu_assert_string_eq("T6o", str);
	c_toString(combos[13 * 8 + 5], str);
	mu_assert_string_eq("96o", str);
	c_toString(combos[13 * 8 + 6], str);
	mu_assert_string_eq("86o", str);
	c_toString(combos[13 * 8 + 7], str);
	mu_assert_string_eq("76o", str);
	c_toString(combos[13 * 8 + 8], str);
	mu_assert_string_eq("66", str);
	c_toString(combos[13 * 8 + 9], str);
	mu_assert_string_eq("65s", str);
	c_toString(combos[13 * 8 + 10], str);
	mu_assert_string_eq("64s", str);
	c_toString(combos[13 * 8 + 11], str);
	mu_assert_string_eq("63s", str);
	c_toString(combos[13 * 8 + 12], str);
	mu_assert_string_eq("62s", str);

	c_toString(combos[13 * 9 + 0], str);
	mu_assert_string_eq("A5o", str);
	c_toString(combos[13 * 9 + 1], str);
	mu_assert_string_eq("K5o", str);
	c_toString(combos[13 * 9 + 2], str);
	mu_assert_string_eq("Q5o", str);
	c_toString(combos[13 * 9 + 3], str);
	mu_assert_string_eq("J5o", str);
	c_toString(combos[13 * 9 + 4], str);
	mu_assert_string_eq("T5o", str);
	c_toString(combos[13 * 9 + 5], str);
	mu_assert_string_eq("95o", str);
	c_toString(combos[13 * 9 + 6], str);
	mu_assert_string_eq("85o", str);
	c_toString(combos[13 * 9 + 7], str);
	mu_assert_string_eq("75o", str);
	c_toString(combos[13 * 9 + 8], str);
	mu_assert_string_eq("65o", str);
	c_toString(combos[13 * 9 + 9], str);
	mu_assert_string_eq("55", str);
	c_toString(combos[13 * 9 + 10], str);
	mu_assert_string_eq("54s", str);
	c_toString(combos[13 * 9 + 11], str);
	mu_assert_string_eq("53s", str);
	c_toString(combos[13 * 9 + 12], str);
	mu_assert_string_eq("52s", str);

	c_toString(combos[13 * 10 + 0], str);
	mu_assert_string_eq("A4o", str);
	c_toString(combos[13 * 10 + 1], str);
	mu_assert_string_eq("K4o", str);
	c_toString(combos[13 * 10 + 2], str);
	mu_assert_string_eq("Q4o", str);
	c_toString(combos[13 * 10 + 3], str);
	mu_assert_string_eq("J4o", str);
	c_toString(combos[13 * 10 + 4], str);
	mu_assert_string_eq("T4o", str);
	c_toString(combos[13 * 10 + 5], str);
	mu_assert_string_eq("94o", str);
	c_toString(combos[13 * 10 + 6], str);
	mu_assert_string_eq("84o", str);
	c_toString(combos[13 * 10 + 7], str);
	mu_assert_string_eq("74o", str);
	c_toString(combos[13 * 10 + 8], str);
	mu_assert_string_eq("64o", str);
	c_toString(combos[13 * 10 + 9], str);
	mu_assert_string_eq("54o", str);
	c_toString(combos[13 * 10 + 10], str);
	mu_assert_string_eq("44", str);
	c_toString(combos[13 * 10 + 11], str);
	mu_assert_string_eq("43s", str);
	c_toString(combos[13 * 10 + 12], str);
	mu_assert_string_eq("42s", str);

	c_toString(combos[13 * 11 + 0], str);
	mu_assert_string_eq("A3o", str);
	c_toString(combos[13 * 11 + 1], str);
	mu_assert_string_eq("K3o", str);
	c_toString(combos[13 * 11 + 2], str);
	mu_assert_string_eq("Q3o", str);
	c_toString(combos[13 * 11 + 3], str);
	mu_assert_string_eq("J3o", str);
	c_toString(combos[13 * 11 + 4], str);
	mu_assert_string_eq("T3o", str);
	c_toString(combos[13 * 11 + 5], str);
	mu_assert_string_eq("93o", str);
	c_toString(combos[13 * 11 + 6], str);
	mu_assert_string_eq("83o", str);
	c_toString(combos[13 * 11 + 7], str);
	mu_assert_string_eq("73o", str);
	c_toString(combos[13 * 11 + 8], str);
	mu_assert_string_eq("63o", str);
	c_toString(combos[13 * 11 + 9], str);
	mu_assert_string_eq("53o", str);
	c_toString(combos[13 * 11 + 10], str);
	mu_assert_string_eq("43o", str);
	c_toString(combos[13 * 11 + 11], str);
	mu_assert_string_eq("33", str);
	c_toString(combos[13 * 11 + 12], str);
	mu_assert_string_eq("32s", str);

	c_toString(combos[13 * 12 + 0], str);
	mu_assert_string_eq("A2o", str);
	c_toString(combos[13 * 12 + 1], str);
	mu_assert_string_eq("K2o", str);
	c_toString(combos[13 * 12 + 2], str);
	mu_assert_string_eq("Q2o", str);
	c_toString(combos[13 * 12 + 3], str);
	mu_assert_string_eq("J2o", str);
	c_toString(combos[13 * 12 + 4], str);
	mu_assert_string_eq("T2o", str);
	c_toString(combos[13 * 12 + 5], str);
	mu_assert_string_eq("92o", str);
	c_toString(combos[13 * 12 + 6], str);
	mu_assert_string_eq("82o", str);
	c_toString(combos[13 * 12 + 7], str);
	mu_assert_string_eq("72o", str);
	c_toString(combos[13 * 12 + 8], str);
	mu_assert_string_eq("62o", str);
	c_toString(combos[13 * 12 + 9], str);
	mu_assert_string_eq("52o", str);
	c_toString(combos[13 * 12 + 10], str);
	mu_assert_string_eq("42o", str);
	c_toString(combos[13 * 12 + 11], str);
	mu_assert_string_eq("32o", str);
	c_toString(combos[13 * 12 + 12], str);
	mu_assert_string_eq("22", str);
}

MU_TEST(combo_pair_has_hands) {
	for (int i = 0; i < 13; i++) {
		combo_t combo = combos[i * 13 + i];

		hand_t pairHands[6];
		pairHands[0] = h_newHand(d_newCard(i, SPADE), d_newCard(i, HEART));
		pairHands[1] = h_newHand(d_newCard(i, SPADE), d_newCard(i, DIAMOND));
		pairHands[2] = h_newHand(d_newCard(i, SPADE), d_newCard(i, CLUB));
		pairHands[3] = h_newHand(d_newCard(i, HEART), d_newCard(i, DIAMOND));
		pairHands[4] = h_newHand(d_newCard(i, HEART), d_newCard(i, CLUB));
		pairHands[5] = h_newHand(d_newCard(i, DIAMOND), d_newCard(i, CLUB));

		ll_t ptr = ll_head(combo.hands);
		for (int x = 0; x < 6; x++) {
			hand_t comboHand;
			ll_get(ptr, &comboHand);
			assert_hand_eq(pairHands[x], comboHand);
			ptr = ll_next(ptr);
		}
	}
}

MU_TEST(combo_suited_has_hands) {
	for (int i = 0; i < 13; i++) {
		for (int j = i + 1; j < 13; j++) {
			combo_t combo = combos[i * 13 + j];

			hand_t suitedHands[4];
			suitedHands[0] = h_newHand(d_newCard(i, SPADE), d_newCard(j, SPADE));
			suitedHands[1] = h_newHand(d_newCard(i, HEART), d_newCard(j, HEART));
			suitedHands[2] = h_newHand(d_newCard(i, DIAMOND), d_newCard(j, DIAMOND));
			suitedHands[3] = h_newHand(d_newCard(i, CLUB), d_newCard(j, CLUB));

			ll_t ptr = ll_head(combo.hands);
			for (int x = 0; x < 4; x++) {
				hand_t comboHand;
				ll_get(ptr, &comboHand);
				assert_hand_eq(suitedHands[x], comboHand);
				ptr = ll_next(ptr);
			}
		}
	}
}

MU_TEST(combo_offsuit_has_hands) {
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < i; j++) {
			combo_t combo = combos[i * 13 + j];

			hand_t offsuitHands[12];
			offsuitHands[0] = h_newHand(d_newCard(j, SPADE), d_newCard(i, HEART));
			offsuitHands[1] = h_newHand(d_newCard(j, SPADE), d_newCard(i, DIAMOND));
			offsuitHands[2] = h_newHand(d_newCard(j, SPADE), d_newCard(i, CLUB));
			offsuitHands[3] = h_newHand(d_newCard(j, HEART), d_newCard(i, SPADE));
			offsuitHands[4] = h_newHand(d_newCard(j, HEART), d_newCard(i, DIAMOND));
			offsuitHands[5] = h_newHand(d_newCard(j, HEART), d_newCard(i, CLUB));
			offsuitHands[6] = h_newHand(d_newCard(j, DIAMOND), d_newCard(i, SPADE));
			offsuitHands[7] = h_newHand(d_newCard(j, DIAMOND), d_newCard(i, HEART));
			offsuitHands[8] = h_newHand(d_newCard(j, DIAMOND), d_newCard(i, CLUB));
			offsuitHands[9] = h_newHand(d_newCard(j, CLUB), d_newCard(i, SPADE));
			offsuitHands[10] = h_newHand(d_newCard(j, CLUB), d_newCard(i, HEART));
			offsuitHands[11] = h_newHand(d_newCard(j, CLUB), d_newCard(i, DIAMOND));

			ll_t ptr = ll_head(combo.hands);
			for (int x = 0; x < 12; x++) {
				hand_t comboHand;
				ll_get(ptr, &comboHand);
				assert_hand_eq(offsuitHands[x], comboHand);
				ptr = ll_next(ptr);
			}
		}
	}
}

MU_TEST(combo_delete_hand) {
	combo_t combosCopy[169];
	for (int i = 0; i < 169; i++) {
		combo_t combo = combos[i];
		combo_t * comboCopy = combosCopy + i;
		*comboCopy = c_newNullCombo();

		comboCopy->ranks[0] = combo.ranks[0];
		comboCopy->ranks[1] = combo.ranks[1];
		comboCopy->type = combo.type;
		
		unsigned max = 0;
		if (combo.type == PAIR) {
			max = 6;
		} else {
			max = (combo.type == SUITED) ? 4 : 12;
		}

		ll_t ptr = ll_head(combo.hands);
		for (unsigned x = 0; x < max; x++) {
			hand_t comboHand;
			ll_get(ptr, &comboHand);
			comboCopy->hands = ll_add(comboCopy->hands, h_encode(comboHand));
			ptr = ll_next(ptr);
		}
	}

	card_t deadCards[7];
	deadCards[0] = d_newCard(ACE, SPADE);
	deadCards[1] = d_newCard(ACE, HEART);
	deadCards[2] = d_newCard(ACE, DIAMOND);
	deadCards[3] = d_newCard(ACE, CLUB);
	deadCards[4] = d_newCard(SEVEN, SPADE);
	deadCards[5] = d_newCard(SEVEN, CLUB);
	deadCards[6] = d_newCard(TWO, HEART);

	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++) {
			combo_t combo = combosCopy[i * 13 + j];

			combo = c_deleteCards(combo, deadCards, 7);

			if (i == ACE || j == ACE) {
				mu_assert_int_eq(0, combo.hands.length);
			} else if (i == SEVEN && j == TWO || i == TWO && j == SEVEN) {
				switch (combo.type) {
					case SUITED: mu_assert_int_eq(1, combo.hands.length); break;
					case OFFSUIT: mu_assert_int_eq(5, combo.hands.length); break;
				}
			} else if (i == SEVEN || j == SEVEN) {
				switch (combo.type) {
					case PAIR: mu_assert_int_eq(1, combo.hands.length); break;
					case SUITED: mu_assert_int_eq(2, combo.hands.length); break;
					case OFFSUIT: mu_assert_int_eq(6, combo.hands.length); break;
				}
			} else if (i == TWO || j == TWO) {
				switch (combo.type) {
					case PAIR: mu_assert_int_eq(3, combo.hands.length); break;
					case SUITED: mu_assert_int_eq(3, combo.hands.length); break;
					case OFFSUIT: mu_assert_int_eq(9, combo.hands.length); break;
				}
			}

			combosCopy[i * 13 + j] = combo;
		}
	}

	for (int i = 0; i < 169; i++) {
		c_deleteCombo(combosCopy[i]);
	}
}

MU_TEST_SUITE(combo_test_suite) {
	MU_RUN_TEST(combo_null);
	MU_RUN_TEST(combo_initialize_pair);
	MU_RUN_TEST(combo_initialize_suited);
	MU_RUN_TEST(combo_initialize_offsuit);
	MU_RUN_TEST(combo_test_toString);
	MU_RUN_TEST(combo_test_toString);
	MU_RUN_TEST(combo_pair_has_hands);
	MU_RUN_TEST(combo_suited_has_hands);
	MU_RUN_TEST(combo_offsuit_has_hands);
	MU_RUN_TEST(combo_delete_hand);
}
