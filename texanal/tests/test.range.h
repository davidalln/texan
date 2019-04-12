#pragma once

#include "range.h"

MU_TEST(range_initialize) {
	range_t range = r_newRange();
	mu_check(r_isNull(range));
	r_deleteRange(range);
}

MU_TEST(range_add_combo) {
	range_t range = r_newRange();
	range = r_addCombo(range, c_newCombo(ACE, ACE));
	mu_check(!r_isNull(range));
	mu_assert_int_eq(1, range.combos.length);
	r_deleteRange(range);
}

MU_TEST(range_add_multiple) {
	range_t range = r_newRange();

	combo_t combos[5];
	combos[0] = c_newCombo(ACE, QUEEN);
	combos[1] = c_newCombo(QUEEN, JACK);
	combos[2] = c_newCombo(JACK, QUEEN);
	combos[3] = c_newCombo(QUEEN, TEN);
	combos[4] = c_newCombo(TWO, TWO);

	range = r_addCombo(range, combos[2]);
	range = r_addCombo(range, combos[1]);
	range = r_addCombo(range, combos[4]);
	range = r_addCombo(range, combos[0]);
	range = r_addCombo(range, combos[3]);

	mu_assert_int_eq(5, range.combos.length);

	combo_t combo = c_newNullCombo();
	combo_t combosFromList[5];
	range.combos = ll_head(range.combos);

	for (int i = 0; i < 5; i++) {
		ll_get(range.combos, &combo);
		combosFromList[i] = combo;
		range.combos = ll_next(range.combos);
	}

	for (int i = 0; i < 4; i++) {
		mu_assert_int_eq(1, c_compare(combosFromList[i], combosFromList[i + 1]));
	}

	r_deleteRange(range);
	for (int i = 0; i < 5; i++) {
		c_deleteCombo(combosFromList[i]);
	}
}

MU_TEST_SUITE(range_test_suite) {
	MU_RUN_TEST(range_initialize);
	MU_RUN_TEST(range_add_combo);
	MU_RUN_TEST(range_add_multiple);
}
