#pragma once

#include "combo.h"

#define MAX_RANGE_NAME_SIZE 64

typedef struct {
	ll_t combos;
	char name[MAX_RANGE_NAME_SIZE];
} range_t;

range_t * r_newRange();
unsigned r_isNull(range_t range);
void r_deleteRange(range_t * range);

unsigned r_addCombo(range_t * range, combo_t combo);
unsigned r_hasCombo(range_t * range, combo_t combo);

range_t r_deleteCards(range_t range, card_t * cards, unsigned nCards);