#pragma once

#include "hand.h"

typedef struct {
	card_t card0;
	card_t card1;
	card_t card2;
} flop_t;

flop_t f_newFlop();
void f_printString(flop_t flop);