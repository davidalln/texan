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
combo_t c_newBlankCombo(rank_t rank0, rank_t rank1, combo_type_t type);
combo_t c_newCombo(rank_t rank0, rank_t rank1);
void c_deleteCombo(combo_t combo);
unsigned c_isNull(combo_t combo);

data c_encode(combo_t combo);
void c_decode(data _data, void* _combo);

unsigned c_addHand(combo_t * combo, hand_t hand);
combo_t c_deleteCards(combo_t combo, card_t * cards, unsigned nCards);

signed c_compare(combo_t a, combo_t b);
signed c_compareRankAndType(combo_t a, combo_t b);
signed c_compareData(data _a, data _b);

void c_toString(combo_t combo, char * str);
