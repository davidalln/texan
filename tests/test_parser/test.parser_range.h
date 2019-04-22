#pragma once

#include "load_file.h"

#include "range.h"

MU_TEST(rp_parse_data_test) {
	char * data = texan_load_file(filename);
	mu_check(data != NULL);

	range_t * range = r_newRange();

	unsigned result = rp_parseRangeJSON(data, range);

	mu_check(result > 0);
	mu_assert_string_eq("EP open", range->name);

	mu_check(r_hasCombo(range, c_newCombo(ACE, ACE)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, KING)));
	mu_check(r_hasCombo(range, c_newCombo(KING, ACE)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, QUEEN)));
	mu_check(r_hasCombo(range, c_newCombo(QUEEN, ACE)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, JACK)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, TEN)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, NINE)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, EIGHT)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, SEVEN)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, SIX)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, FIVE)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, FOUR)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, THREE)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, TWO)));
	mu_check(r_hasCombo(range, c_newCombo(KING, KING)));
	mu_check(r_hasCombo(range, c_newCombo(KING, QUEEN)));
	mu_check(r_hasCombo(range, c_newCombo(KING, JACK)));
	mu_check(r_hasCombo(range, c_newCombo(QUEEN, QUEEN)));
	mu_check(r_hasCombo(range, c_newCombo(QUEEN, JACK)));
	mu_check(r_hasCombo(range, c_newCombo(JACK, JACK)));
	mu_check(r_hasCombo(range, c_newCombo(JACK, TEN)));
	mu_check(r_hasCombo(range, c_newCombo(TEN, TEN)));
	mu_check(r_hasCombo(range, c_newCombo(TEN, NINE)));
	mu_check(r_hasCombo(range, c_newCombo(NINE, NINE)));
	mu_check(r_hasCombo(range, c_newCombo(NINE, EIGHT)));
	mu_check(r_hasCombo(range, c_newCombo(EIGHT, EIGHT)));
	mu_check(r_hasCombo(range, c_newCombo(SEVEN, SEVEN)));
	mu_check(r_hasCombo(range, c_newCombo(SIX, SIX)));
	mu_check(r_hasCombo(range, c_newCombo(FIVE, FIVE)));
	mu_check(r_hasCombo(range, c_newCombo(FOUR, FOUR)));
	mu_check(r_hasCombo(range, c_newCombo(THREE, THREE)));
	mu_check(r_hasCombo(range, c_newCombo(TWO, TWO)));


	r_deleteRange(range);
	free(data);
}

MU_TEST_SUITE(rp_suite) {
	MU_RUN_TEST(rp_parse_data_test);
}