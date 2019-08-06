#pragma once

#include "load_file.h"

#include "range.h"

MU_TEST(rp_parse_data_test) {
	char * data = texan_load_file(filename);
	mu_check(data != NULL);

	range_t * range = r_newRange();

	unsigned result = rp_parseRangeJSON(data, range);

	mu_assert_int_eq(33, result);
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

MU_TEST(rp_parse_range_string_test) {
	range_t * range = r_newRange();

	unsigned result = rp_parseRangeString("AKs, AKo", range);
	mu_assert_int_eq(2, result);

	mu_check(r_hasCombo(range, c_newCombo(ACE, KING)));
	mu_check(r_hasCombo(range, c_newCombo(KING, ACE)));

	r_deleteRange(range);

	range = r_newRange();

	result = rp_parseRangeString("22+, ATs+, 72o+", range);
	mu_assert_int_eq(22, result);

	mu_check(r_hasCombo(range, c_newCombo(ACE, ACE)));
	mu_check(r_hasCombo(range, c_newCombo(KING, KING)));
	mu_check(r_hasCombo(range, c_newCombo(QUEEN, QUEEN)));
	mu_check(r_hasCombo(range, c_newCombo(JACK, JACK)));
	mu_check(r_hasCombo(range, c_newCombo(TEN, TEN)));
	mu_check(r_hasCombo(range, c_newCombo(NINE, NINE)));
	mu_check(r_hasCombo(range, c_newCombo(EIGHT, EIGHT)));
	mu_check(r_hasCombo(range, c_newCombo(SEVEN, SEVEN)));
	mu_check(r_hasCombo(range, c_newCombo(SIX, SIX)));
	mu_check(r_hasCombo(range, c_newCombo(FIVE, FIVE)));
	mu_check(r_hasCombo(range, c_newCombo(FOUR, FOUR)));
	mu_check(r_hasCombo(range, c_newCombo(THREE, THREE)));
	mu_check(r_hasCombo(range, c_newCombo(TWO, TWO)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, TEN)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, JACK)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, QUEEN)));
	mu_check(r_hasCombo(range, c_newCombo(ACE, KING)));
	mu_check(r_hasCombo(range, c_newCombo(TWO, SEVEN)));
	mu_check(r_hasCombo(range, c_newCombo(THREE, SEVEN)));
	mu_check(r_hasCombo(range, c_newCombo(FOUR, SEVEN)));
	mu_check(r_hasCombo(range, c_newCombo(FIVE, SEVEN)));
	mu_check(r_hasCombo(range, c_newCombo(SIX, SEVEN)));

	r_deleteRange(range);

	range = r_newRange();

	result = rp_parseRangeString("66-33, QTs-Q2s, 95o-92o, 97o+", range);
	mu_assert_int_eq(19, result);

	mu_check(r_hasCombo(range, c_newCombo(SIX, SIX)));
	mu_check(r_hasCombo(range, c_newCombo(FIVE, FIVE)));
	mu_check(r_hasCombo(range, c_newCombo(FOUR, FOUR)));
	mu_check(r_hasCombo(range, c_newCombo(THREE, THREE)));
	mu_check(r_hasCombo(range, c_newCombo(QUEEN, TEN)));
	mu_check(r_hasCombo(range, c_newCombo(QUEEN, NINE)));
	mu_check(r_hasCombo(range, c_newCombo(QUEEN, EIGHT)));
	mu_check(r_hasCombo(range, c_newCombo(QUEEN, SEVEN)));
	mu_check(r_hasCombo(range, c_newCombo(QUEEN, SIX)));
	mu_check(r_hasCombo(range, c_newCombo(QUEEN, FIVE)));
	mu_check(r_hasCombo(range, c_newCombo(QUEEN, FOUR)));
	mu_check(r_hasCombo(range, c_newCombo(QUEEN, THREE)));
	mu_check(r_hasCombo(range, c_newCombo(QUEEN, TWO)));
	mu_check(r_hasCombo(range, c_newCombo(TWO, NINE)));
	mu_check(r_hasCombo(range, c_newCombo(THREE, NINE)));
	mu_check(r_hasCombo(range, c_newCombo(FOUR, NINE)));
	mu_check(r_hasCombo(range, c_newCombo(FIVE, NINE)));
	mu_check(r_hasCombo(range, c_newCombo(SEVEN, NINE)));
	mu_check(r_hasCombo(range, c_newCombo(EIGHT, NINE)));

	r_deleteRange(range);
}

MU_TEST(rp_encode_range_test) {
	range_t * range = r_newRange();
	unsigned result = rp_parseRangeString("22+, A2s+, A2o+, K4s+, KTo+, Q7s+, QTo+, J7s+, J9o+, T6s+, 98o, 96s+, 87o, 86s+, 75s+, 64s+, 54s+, 43s", range);
	printf("asdf");
}

MU_TEST_SUITE(rp_suite) {
	MU_RUN_TEST(rp_parse_data_test);
	MU_RUN_TEST(rp_parse_range_string_test);
	MU_RUN_TEST(rp_encode_range_test);
}