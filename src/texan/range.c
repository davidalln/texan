#include <stdio.h>
#include <stdlib.h>

#include "ll.h"
#include "range.h"

range_report_t * rr_newNullReport();
void rr_deleteReport(range_report_t * report);

range_t * r_newRange() {
	range_t * range = (range_t *)malloc(sizeof(range_t));

	range->combos = ll_newList();
	range->combos.decode = c_decode;
	range->combos.compare = c_compareData;

	range->name[0] = '\0';

	range->nHands = 0;

	range->report = rr_newNullReport();

	return range;
}

unsigned r_isNull(range_t range) {
	return ll_isNull(range.combos);
}

void r_deleteRange(range_t * range) {
	ll_deleteList(range->combos);
	rr_deleteReport(range->report);
	free(range);
}

unsigned r_addCombo(range_t * range, combo_t combo) {
	if (r_hasCombo(range, combo)) {
		combo_t oldCombo = c_newNullCombo();
		ll_get(range->combos, &oldCombo);

		range->combos = ll_delete(range->combos);
		range->nHands -= combo.hands.length;

		combo = c_mergeCombo(combo, oldCombo);
	}

	range->combos = ll_add(range->combos, c_encode(combo));
	range->nHands += combo.hands.length;

	return range->combos.length;
}

unsigned r_hasCombo(range_t * range, combo_t combo) {
	if (ll_isNull(range->combos)) {
		return 0;
	}

	range->combos = ll_search(range->combos, c_encode(combo));

	combo_t searchCombo = c_newNullCombo();
	ll_get(range->combos, &searchCombo);

	signed cmp = (c_compareRankAndType(searchCombo, combo));
	c_deleteCombo(searchCombo);

	return (cmp == 0);
}

unsigned r_deleteCards_hands(range_t * range, card_t * cards, unsigned nCards, hand_t * removedHands, unsigned * nHandsDeleted) {
	unsigned total_nhd = 0;

	range->combos = ll_head(range->combos);
	range->nHands = 0;

	unsigned combos_length = range->combos.length;

	for (unsigned i = 0; i < combos_length; i++) {
		combo_t combo = c_newNullCombo();
		ll_get(range->combos, &combo);

		unsigned old_length = combo.hands.length;

		if (removedHands != NULL) {
			hand_t rmh[16];
			unsigned nhd = 0;

			combo = c_deleteCards_hands(combo, cards, nCards, rmh, &nhd);
			for (int j = 0; j < nhd; j++) {
				removedHands[total_nhd++] = rmh[j];
			}
		}
		else {
			combo = c_deleteCards(combo, cards, nCards);
		}

		if (combo.hands.length == 0) {
			range->combos = ll_delete(range->combos);
		}
		else {
			range->combos.ptr->payload = c_encode(combo);
			range->combos = ll_next(range->combos);
		}

		char qstr[10];
		c_toString(combo, qstr);
		//printf("combo %s now has length %d (old: %d)\n", qstr, combo.hands.length, old_length);
		range->nHands += combo.hands.length;

		c_deleteCombo(combo);
	}

	if (nHandsDeleted != NULL) {
		*nHandsDeleted = total_nhd;
	}

	return range->nHands;
}

unsigned r_deleteCards(range_t * range, card_t * cards, unsigned nCards) {
	return r_deleteCards_hands(range, cards, nCards, NULL, NULL);
}

unsigned r_generateReport(range_t * range, card_t * cards, unsigned nCards) {
	range->report = rr_newNullReport();
	hand_t * hands = (hand_t *)malloc(sizeof(hand_t) * range->nHands);

	range->combos = ll_head(range->combos);
	unsigned combos_length = range->combos.length;
	unsigned hand_idx = 0;

	for (unsigned i = 0; i < combos_length; i++) {
		combo_t combo = c_newNullCombo();
		ll_get(range->combos, &combo);

		combo.hands = ll_head(combo.hands);
		unsigned hands_length = combo.hands.length;
		for (int i = 0; i < hands_length; i++) {
			hand_t hand = h_newNullHand();
			ll_get(combo.hands, &hand);

			hands[hand_idx++] = h_calcStrength(hand, cards, nCards);
			combo.hands = ll_next(combo.hands);
		}

		range->combos = ll_next(range->combos);
		c_deleteCombo(combo);
	}

	for (int i = 0; i < range->nHands; i++) {
		hand_t x = hands[i];
		unsigned idx = i;

		for (int j = i + 1; j < range->nHands; j++) {
			if (x.strength > hands[j].strength) {
				x = hands[j];
				idx = j;
			}
		}

		hand_t tmp = hands[i];
		hands[i] = x;
		hands[idx] = tmp;
	}

	// value
	unsigned maxValue = (unsigned)(range->nHands * 0.3);
	unsigned strength_cutoff = hands[maxValue].strength;
	range->report->value_hands = (range_report_t *)malloc(sizeof(range_report_t) * maxValue);
	for (int i = 0; i < maxValue; i++) {
		range->report->value_hands[i] = hands[i];
	}
	range->report->nValue_hands = maxValue;

	for (int i = 0; i < range->report->nValue_hands; i++) {
		hand_t x = range->report->value_hands[i];
		char handstr[10];
		h_toString(x, handstr);
		printf("hand %s has strength %d\n", handstr, x.strength);
	}

	free(hands);
	return range->nHands;
}

range_report_t * rr_newNullReport() {
	range_report_t * report = (range_report_t*)malloc(sizeof(range_report_t));

	report->value_hands = NULL;
	report->draw_hands = NULL;
	report->fold_hands = NULL;
	report->nValue_hands = 0;
	report->nDraw_hands = 0;
	report->nFold_hands = 0;

	return report;
}

void rr_deleteReport(range_report_t * report) {
	free(report->value_hands);
	free(report->draw_hands);
	free(report->fold_hands);
	free(report);
}

void rr_addHands(range_report_t * report, combo_t combo, card_t * cards, unsigned nCards) {

}

hand_t * r_generateHandArray(range_t * range) {
	hand_t *hands = malloc(sizeof(hand_t) * range->nHands);
	unsigned hand_idx = 0;
	printf("there should be %d hands...\n", range->nHands);

	range->combos = ll_head(range->combos);
	do {
		combo_t combo = c_newNullCombo();
		ll_get(range->combos, &combo);

		combo.hands = ll_head(combo.hands);
		do {
			hand_t hand;
			ll_get(combo.hands, &hand);
			hands[hand_idx++] = hand;
			combo.hands = ll_next(combo.hands);
			char qstr[10];
			h_toString(hand, qstr);
			printf("\tadded hand %s to hand array\n", qstr);
		} while (!ll_atHead(combo.hands));

		range->combos = ll_next(range->combos);
	} while (!ll_atHead(range->combos));

	return hands;
}