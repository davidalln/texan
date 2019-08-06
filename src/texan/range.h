#pragma once

#include "combo.h"

#define MAX_RANGE_NAME_SIZE 64

typedef struct {
	hand_t * value_hands;
	size_t nValue_hands;

	hand_t * draw_hands;
	size_t nDraw_hands;

	hand_t * fold_hands;
	size_t nFold_hands;
} range_report_t;

typedef struct {
	ll_t combos;
	range_report_t * report;
	size_t nHands;
	char name[MAX_RANGE_NAME_SIZE];
} range_t;


range_t * r_newRange();
unsigned r_isNull(range_t range);
void r_deleteRange(range_t * range);

unsigned r_addCombo(range_t * range, combo_t combo);
unsigned r_hasCombo(range_t * range, combo_t combo);

unsigned r_deleteCards_hands(range_t * range, card_t * cards, unsigned nCards, hand_t * removedHands, unsigned * nHandsDeleted);
unsigned r_deleteCards(range_t * range, card_t * cards, unsigned nCards);
unsigned r_generateReport(range_t * range, card_t * cards, unsigned nCards);

hand_t * r_generateHandArray(range_t * range);