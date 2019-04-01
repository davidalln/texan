#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef unsigned long data;

typedef enum {
	ACE, KING, QUEEN, JACK, TEN, NINE, EIGHT, SEVEN, SIX, FIVE, FOUR, THREE, TWO, NO_RANK
} rank_t;

typedef enum {
	PAIR, SUITED, OFFSUIT, MIXED, NO_TYPE
} comboType_t;

typedef struct {
	data definition;				// [rank0][rank1][nHandsActive][type]
	data handsActive;
} combo_t;

rank_t c_rank0(combo_t combo);
rank_t c_rank1(combo_t combo);
unsigned int c_nHandsActive(combo_t combo);
comboType_t c_type(combo_t combo);

combo_t c_setRanks(combo_t combo, rank_t rank0, rank_t rank1);

void c_printString(combo_t combo);

combo_t c_newBlankCombo();
combo_t c_newCombo(rank_t rank0, rank_t rank1);
combo_t c_mergeCombo(combo_t combo0, combo_t combo1);

data c_handsOfType(combo_t combo, comboType_t type);