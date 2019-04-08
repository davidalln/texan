#include "range.h"
#include "style.h"


range_t r_newRange()
{
	range_t range;

	range.comboList = (r_node_t *)(malloc(sizeof(r_node_t)));
	range.comboList->combo = c_newBlankCombo();
	range.comboList->prev = NULL;
	range.comboList->next = NULL;
	range.comboList->handList = NULL;

	range.searchPtr = range.comboList;

	return range;
}

r_node_t * searchRangeForCombo(r_node_t *searchPtr, combo_t combo) {
	if (searchPtr) {
		if (c_rank0(searchPtr->combo) != c_rank0(combo)) {
			while (searchPtr->next && (c_rank0(searchPtr->next->combo) <= c_rank0(combo))) {
				if (searchPtr->next && (c_rank0(searchPtr->next->combo) == c_rank0(combo))) {
					searchPtr = searchPtr->next;
					break;
				}

				searchPtr = searchPtr->next;
			}
			while (searchPtr->prev && (c_rank0(searchPtr->prev->combo) >= c_rank0(combo))) {
				if (searchPtr->prev && (c_rank0(searchPtr->prev->combo) == c_rank0(combo))) {
					searchPtr = searchPtr->prev;
					break;
				}

				searchPtr = searchPtr->prev;
			}
		}

		if (c_rank0(searchPtr->combo) == c_rank0(combo)) {
			while (searchPtr->next && (c_rank1(searchPtr->combo) < c_rank1(combo))) {
				
				if (searchPtr->next && (c_rank0(searchPtr->next->combo) > c_rank0(combo))) {
					break;
				}
				else {
					searchPtr = searchPtr->next;
					if (searchPtr->next && (c_rank0(searchPtr->next->combo) > c_rank0(combo))) {
						break;
					}
					else if (searchPtr->next && (c_rank1(searchPtr->next->combo) == c_rank1(combo))) {
						searchPtr = searchPtr->next;
						break;
					}
				}
			}
			while (searchPtr->prev && (c_rank1(searchPtr->combo) > c_rank1(combo))) {
				if (searchPtr->prev && (c_rank0(searchPtr->prev->combo) < c_rank0(combo))) {
					break;
				}
				else {
					searchPtr = searchPtr->prev;
					if (searchPtr->prev && (c_rank0(searchPtr->prev->combo) < c_rank0(combo))) {
						break;
					}
					else if (searchPtr->prev && (c_rank1(searchPtr->prev->combo) == c_rank1(combo))) {
						searchPtr = searchPtr->prev;
						break;
					}
				}
			}
		}
	}
	
	return searchPtr;
}

range_t addComboToRange(range_t range, combo_t combo) {
	r_node_t * searchPtr = range.searchPtr;
	if (searchPtr && c_type(searchPtr->combo) == NO_TYPE) {
		searchPtr->combo = combo;
		return range;
	}

	printf("adding combo ");
	c_printString(combo);
	printf(" to range with nHands %d", c_nHandsActive(combo));

	searchPtr = searchRangeForCombo(searchPtr, combo);


	if (c_rank0(searchPtr->combo) == c_rank0(combo) && c_rank1(searchPtr->combo) == c_rank1(combo)) {
		printf("... and merging with ");
		c_printString(searchPtr->combo);
		printf(" -> (");
		searchPtr->combo = c_mergeCombo(combo, searchPtr->combo);
		c_printString(searchPtr->combo);
		printf(")\n");

	}
	else {
		r_node_t *newCombo = (r_node_t *)(malloc(sizeof(r_node_t)));
		newCombo->combo = combo;
		newCombo->handList = NULL;

		if (searchPtr->next) {
			newCombo->prev = searchPtr;
			newCombo->next = searchPtr->next;
			newCombo->next->prev = newCombo;
			newCombo->prev->next = newCombo;
		}
		else {
			searchPtr->next = newCombo;
			newCombo->prev = searchPtr;
			newCombo->next = NULL;
		}
	}
	printf("\n");

	range.searchPtr = searchPtr;
	return range;
}

range_t r_fillHandList(range_t range) {
	for (r_node_t * node = range.comboList; node != NULL; node = node->next) {
		data handsActive = node->combo.handsActive;

		if (handsActive > 0) {
			if (!node->handList) {
				node->handList = (h_node_t *)malloc(sizeof(h_node_t));
				node->handList->hand = h_newHand(NO_RANK, NO_SUIT, NO_RANK, NO_SUIT);
				node->handList->next = NULL;
				node->handList->prev = NULL;
			}

			h_node_t * searchPtr = node->handList;
			unsigned seenHand = 0;
			for (unsigned i = 0; i < HANDS_PER_COMBO; i++) {
				unsigned handActive = handsActive & (1 << (HANDS_PER_COMBO - i - 1));
				if (handActive) {
					suit_t suit0 = i % 4;
					suit_t suit1 = i / 4;

					if (seenHand) {
						searchPtr->next = (h_node_t *)malloc(sizeof(h_node_t));
						searchPtr->next->prev = searchPtr;
						searchPtr = searchPtr->next;
					}

					seenHand = 1;
					searchPtr->hand = h_newHand(c_rank0(node->combo), suit0, c_rank1(node->combo), suit1);
					searchPtr->next = NULL;
				}
			}
		}
		else {
			if (node->handList) {
				free(node->handList);
				node->handList = NULL;
			}
		}
	}

	return range;
}

range_t r_applyDeadCards(range_t range, card_t card0, card_t card1, card_t card2, card_t card3, card_t card4)
{
	for (r_node_t * node = range.comboList; node != NULL;) {
		combo_t combo = c_applyDeadCards(node->combo, card0, card1, card2, card3, card4);
		printf("def: %lu handsActive: %lu combo: ", combo.definition, combo.handsActive);
		c_printString(combo);
		printf("\n");
		node->combo = combo;
		if (node->handList) {
			for (h_node_t * hand = node->handList; hand != NULL;) {
				if (
					(h_cardRank(card0) != NO_RANK && h_hasCard(hand->hand, card0))
					|| (h_cardRank(card1) != NO_RANK && h_hasCard(hand->hand, card1))
					|| (h_cardRank(card2) != NO_RANK && h_hasCard(hand->hand, card2))
					|| (h_cardRank(card3) != NO_RANK && h_hasCard(hand->hand, card3))
					|| (h_cardRank(card4) != NO_RANK && h_hasCard(hand->hand, card4))
					) {
					if (hand->next) hand->next->prev = hand->prev;
					if (hand->prev) hand->prev->next = hand->next;
					else node->handList = hand->next;

					h_node_t * garbage = hand;
					hand = hand->next;
					free(garbage);
				}
				else {
					hand = hand->next;
				}
			}
		}

		if (node->combo.handsActive == 0) {
			r_node_t * garbage = node;
			range.comboList = node->next;
			node->next->prev = NULL;
			node = node->next;
			free(garbage);
		}
		else {
			node = node->next;
		}
	}
	return range;
}

void r_printRange(range_t range) {
	combo_t * combos = (combo_t *)malloc(RANGE_LENGTH * RANGE_LENGTH * sizeof(combo_t));

	for (unsigned i = 0; i < RANGE_LENGTH; i++) {
		for (unsigned j = 0; j < RANGE_LENGTH; j++) {
			unsigned index = i * RANGE_LENGTH + j;
			combos[index] = c_newCombo(i, j);
		}
	}

	r_node_t * searchPtr = range.comboList;
	for (unsigned i = 0; i < RANGE_LENGTH; i++) {
		printf("+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+\n");
		for (unsigned j = 0; j < RANGE_LENGTH; j++) {
			unsigned index = i * RANGE_LENGTH + j;

			combo_t combo = combos[index];
			printf("| ");

			searchPtr = searchRangeForCombo(searchPtr, combo);
			if (searchPtr && (c_rank0(searchPtr->combo) == c_rank0(combo) && c_rank1(searchPtr->combo) == c_rank1(combo))) {
				if (c_handsOfType(searchPtr->combo, c_type(combo))) {
					u_setStyle(BLACK, GREEN);
				}
			}
			c_printString(combo);
			u_resetStyle();

			printf(" ");
		}

		printf("|\n");
	}
	printf("+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+\n");
	printf("\n");
}

void r_printCombos(range_t range)
{
	for (r_node_t * node = range.comboList; node != NULL; node = node->next) {
		if (c_type(node->combo) != NO_TYPE) {
			c_printString(node->combo);

			if (node->handList) {
				printf("(");
				
				for (h_node_t * hand = node->handList; hand != NULL; hand = hand->next) {
					h_printString(hand->hand);
					printf(", ");
				}

				printf(")");
			}
		}

		printf(" ");
	}
	printf("\n");
}
