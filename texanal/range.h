#pragma once

#include "combo.h"

typedef struct combo_list combo_list_t;
struct combo_list {
	combo_t combo;
	combo_list_t *prev;
	combo_list_t *next;
};

typedef struct {
	combo_list_t * combos;
} range_t;
