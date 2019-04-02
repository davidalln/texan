#include "flop.h"

flop_t f_newFlop()
{
	flop_t flop;
	flop.card0 = h_newBlankCard();
	flop.card1 = h_newBlankCard();
	flop.card2 = h_newBlankCard();

	return flop;
}

void f_printString(flop_t flop) {
	if (h_cardRank(flop.card0) != NO_RANK) {
		printf("card0: "); h_cardPrintString(flop.card0); printf(" ");
	}
	if (h_cardRank(flop.card1) != NO_RANK) {
		printf("card1: "); h_cardPrintString(flop.card1); printf(" ");
	}
	if (h_cardRank(flop.card2) != NO_RANK) {
		printf("card2: "); h_cardPrintString(flop.card2);
	}
	printf("\n");
}