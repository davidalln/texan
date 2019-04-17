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

	r_deleteRange(range);
	free(data);
}

MU_TEST_SUITE(rp_suite) {
	MU_RUN_TEST(rp_parse_data_test);
}