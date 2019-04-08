#pragma once

#include "ll.h"
#include "hand.h"

typedef enum {
	PAIR, SUITED, OFFSUIT, NULL_COMBO
} combo_type_t;

typedef struct {
	rank_t ranks[2];
	combo_type_t type;

	ll_t hands;
} combo_t;

combo_t c_newNullCombo();
combo_t c_newCombo(rank_t rank0, rank_t rank1);
void c_deleteCombo();
unsigned c_isNull(combo_t combo);
unsigned c_hasCard(card_t card);

signed c_compare(combo_t a, combo_t b);

void c_toString(combo_t combo, char * str);
