#pragma once

#include "combo.h"

typedef struct {
	ll_t combos;
} range_t;

range_t r_newRange();
unsigned r_isNull(range_t range);
void r_deleteRange(range_t range);

range_t r_addCombo(range_t range, combo_t combo);
