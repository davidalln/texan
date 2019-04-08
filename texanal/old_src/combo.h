#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "hand.h"

#define HANDS_PER_COMBO 16

typedef unsigned short data;

typedef enum {
	PAIR, SUITED, OFFSUIT, MIXED, NO_TYPE
} comboType_t;

typedef struct {
	data definition;				// [rank0][rank1][nHandsActive][type]
	data handsActive;
} combo_t;

rank_t c_rank0(combo_t combo);
rank_t c_rank1(combo_t combo);
unsigned char c_nHandsActive(combo_t combo);
comboType_t c_type(combo_t combo);

combo_t c_setRanks(combo_t combo, rank_t rank0, rank_t rank1);

void c_printString(combo_t combo);

combo_t c_newBlankCombo();
combo_t c_newCombo(rank_t rank0, rank_t rank1);
combo_t c_mergeCombo(combo_t combo0, combo_t combo1);

data c_handsOfType(combo_t combo, comboType_t type);

combo_t c_applyDeadCards(combo_t combo, card_t card0, card_t card1, card_t card2, card_t card3, card_t card4);