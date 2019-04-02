#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "combo.h"
#include "hand.h"

#define RANGE_LENGTH 13

typedef struct h_node_t h_node_t;
struct h_node_t {
	hand_t hand;
	h_node_t *next;
	h_node_t *prev;
};

typedef struct r_node_t r_node_t;
struct r_node_t {
	combo_t combo;
	h_node_t *handList;

	r_node_t *prev;
	r_node_t *next;
};

typedef struct {
	r_node_t * comboList;
	r_node_t * searchPtr;
} range_t ;

range_t r_newRange();
range_t addComboToRange(range_t range, combo_t combo);
range_t r_fillHandList(range_t range);

range_t r_applyDeadCards(range_t range, card_t card0, card_t card1, card_t card2, card_t card3, card_t card4);

void r_printRange(range_t range);
void r_printCombos(range_t range);