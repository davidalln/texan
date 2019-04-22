#pragma once

#include "range.h"

MU_TEST(range_initialize) {
	range_t * range = r_newRange();
	mu_check(r_isNull(*range));
	r_deleteRange(range);
}

MU_TEST(range_add_combo) {
	range_t * range = r_newRange();
	r_addCombo(range, c_newCombo(ACE, ACE));
	mu_check(!r_isNull(*range));
	mu_assert_int_eq(1, range->combos.length);
	r_deleteRange(range);
}

MU_TEST(range_add_multiple) {
	range_t * range = r_newRange();

	combo_t combos[5];
	combos[0] = c_newCombo(ACE, QUEEN);
	combos[1] = c_newCombo(QUEEN, JACK);
	combos[2] = c_newCombo(JACK, QUEEN);
	combos[3] = c_newCombo(QUEEN, TEN);
	combos[4] = c_newCombo(TWO, TWO);

	r_addCombo(range, combos[2]);
	r_addCombo(range, combos[1]);
	r_addCombo(range, combos[4]);
	r_addCombo(range, combos[0]);
	r_addCombo(range, combos[3]);

	mu_assert_int_eq(5, range->combos.length);

	range->combos = ll_head(range->combos);
	do {
		combo_t combo0 = c_newNullCombo();
		combo_t combo1 = c_newNullCombo();

		ll_get(range->combos, &combo0);
		range->combos = ll_next(range->combos);
		ll_get(range->combos, &combo1);

		mu_assert_int_eq(1, c_compare(combo0, combo1));

		c_deleteCombo(combo0);
		c_deleteCombo(combo1);
	} while (!ll_atHead(range->combos) && !ll_atHead(ll_next(range->combos)));

	r_deleteRange(range);
	for (int i = 0; i < 5; i++) {
		c_deleteCombo(combos[i]);
	}
}

MU_TEST(range_has_combo) {
	range_t * range = r_newRange();

	combo_t combos[5];
	combos[0] = c_newCombo(ACE, QUEEN);
	combos[1] = c_newCombo(QUEEN, JACK);
	combos[2] = c_newCombo(JACK, QUEEN);
	combos[3] = c_newCombo(QUEEN, TEN);
	combos[4] = c_newCombo(TWO, TWO);

	r_addCombo(range, combos[3]);
	r_addCombo(range, combos[0]);
	r_addCombo(range, combos[4]);
	r_addCombo(range, combos[1]);
	r_addCombo(range, combos[2]);

	mu_assert_int_eq(5, range->combos.length);

	for (int i = 0; i < 5; i++) {
		mu_check(r_hasCombo(range, combos[i]));
	}

	r_deleteRange(range);
	for (int i = 0; i < 5; i++) {
		c_deleteCombo(combos[i]);
	}
}

MU_TEST(range_delete_cards) {
	range_t * range = r_newRange();

	combo_t combos[5];
	combos[0] = c_newCombo(ACE, ACE);
	combos[1] = c_newCombo(QUEEN, JACK);
	combos[2] = c_newCombo(JACK, QUEEN);
	combos[3] = c_newCombo(QUEEN, TEN);
	combos[4] = c_newCombo(TWO, TWO);

	card_t cards[5];
	cards[0] = d_newCard(ACE, HEART);
	cards[1] = d_newCard(ACE, DIAMOND);
	cards[2] = d_newCard(ACE, CLUB);
	cards[3] = d_newCard(QUEEN, CLUB);
	cards[4] = d_newCard(JACK, SPADE);

	for (int i = 0; i < 5; i++) {
		r_addCombo(range, combos[i]);
	}

	mu_assert_int_eq(5, range->combos.length);

	*range = r_deleteCards(*range, cards, 5);
	mu_assert_int_eq(4, range->combos.length);
	
	combo_t newCombo;
	range->combos = ll_head(range->combos);

	newCombo = c_newNullCombo();
	ll_get(range->combos, &newCombo);
	mu_assert_int_eq(0, c_compareRankAndType(newCombo, combos[1]));
	mu_assert_int_eq(2, newCombo.hands.length);
	c_deleteCombo(newCombo);
	range->combos = ll_next(range->combos);

	newCombo = c_newNullCombo();
	ll_get(range->combos, &newCombo);
	mu_assert_int_eq(0, c_compareRankAndType(newCombo, combos[2]));
	mu_assert_int_eq(7, newCombo.hands.length);
	c_deleteCombo(newCombo);
	range->combos = ll_next(range->combos);

	newCombo = c_newNullCombo();
	ll_get(range->combos, &newCombo);
	mu_assert_int_eq(0, c_compareRankAndType(newCombo, combos[3]));
	mu_assert_int_eq(3, newCombo.hands.length);
	c_deleteCombo(newCombo);
	range->combos = ll_next(range->combos);

	newCombo = c_newNullCombo();
	ll_get(range->combos, &newCombo);
	mu_assert_int_eq(0, c_compareRankAndType(newCombo, combos[4]));
	mu_assert_int_eq(6, newCombo.hands.length);
	c_deleteCombo(newCombo);
	range->combos = ll_next(range->combos);

	mu_check(ll_atHead(range->combos));
}

MU_TEST_SUITE(range_test_suite) {
	MU_RUN_TEST(range_initialize);
	MU_RUN_TEST(range_add_combo);
	MU_RUN_TEST(range_add_multiple);
	MU_RUN_TEST(range_has_combo);
	MU_RUN_TEST(range_delete_cards);
}
