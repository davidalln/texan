#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"

#include "test.ll.h"
#include "test.card.h"
#include "test.hand.h"
#include "test.combo.h"
#include "test.range.h"
#include "test.equity.h"

int main() {
	initialize_cards();
	initialize_hands();
	initialize_combos();

	printf("\n\n== linked list ===============\n");
	MU_RUN_SUITE(linked_list_suite);

	printf("\n\n== card ======================\n");
	MU_RUN_SUITE(card_test_suite);

	printf("\n\n== hand ======================\n");
	MU_RUN_SUITE(hand_test_suite);

	printf("\n\n== combo =====================\n");
	MU_RUN_SUITE(combo_test_suite);

	printf("\n\n== range =====================\n");
	MU_RUN_SUITE(range_test_suite);

	printf("\n\n== equity ====================\n");
	MU_RUN_SUITE(equity_test_suite);

	delete_combos();

	printf("\n\n== report ====================");
	MU_REPORT();

	return minunit_status;
}
