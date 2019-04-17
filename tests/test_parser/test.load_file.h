#pragma once

#include "load_file.h"

MU_TEST(load_file_not_null) {
	char * data = texan_load_file(filename);
	mu_check(data != NULL);
	free(data);
}

MU_TEST_SUITE(load_file_suite) {
	MU_RUN_TEST(load_file_not_null);
}
