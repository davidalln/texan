#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"

const char * filename = "ep_open.range.json";

#include "test.load_file.h"
#include "test.parser_range.h"

int main() {
	printf("\n\n== load file =================\n");
	MU_RUN_SUITE(load_file_suite);

	printf("\n\n== parser.range ==============\n");
	MU_RUN_SUITE(rp_suite);

	printf("\n\n== report ====================");
	MU_REPORT();

	return minunit_status;
}
