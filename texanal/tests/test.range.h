#pragma once

MU_TEST(test_check) {
	mu_check(7 == 5);
}

MU_TEST_SUITE(range_test_suite) {
	MU_RUN_TEST(test_check);
}
