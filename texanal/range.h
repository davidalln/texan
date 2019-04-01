#pragma once

#include <stdlib.h>
#include "combo.h"

#define RANGE_LENGTH 13

typedef struct node_t node_t;
struct node_t {
	combo_t combo;
	node_t *prev;
	node_t *next;
};

typedef struct {
	node_t * comboList;
	node_t * searchPtr;
} range_t ;

range_t r_newRange();
range_t addComboToRange(range_t range, combo_t combo);

void r_printRange(range_t range);
void r_printCombos(range_t range);