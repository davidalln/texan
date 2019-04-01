#include "range.h"
#include "style.h"

range_t r_newRange()
{
	range_t range;

	range.comboList = (node_t *)(malloc(sizeof(node_t)));
	range.comboList->combo = c_newBlankCombo();
	range.comboList->prev = NULL;
	range.comboList->next = NULL;

	range.searchPtr = range.comboList;

	return range;
}

inline node_t * searchRangeForCombo(node_t *searchPtr, combo_t combo) {
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
	node_t * searchPtr = range.searchPtr;
	if (searchPtr && c_type(searchPtr->combo) == NO_TYPE) {
		searchPtr->combo = combo;
		return range;
	}

	searchPtr = searchRangeForCombo(searchPtr, combo);

	node_t *newCombo = (node_t *)(malloc(sizeof(node_t)));
	newCombo->combo = combo;
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

	if (newCombo->prev && (
		c_rank0(newCombo->prev->combo) == c_rank0(newCombo->combo)
		&& c_rank1(newCombo->prev->combo) == c_rank1(newCombo->combo)
		)) {
		newCombo->prev->combo = c_mergeCombo(newCombo->combo, newCombo->prev->combo);
		newCombo->prev->next = newCombo->next;

		if (newCombo->next) {
			newCombo->next->prev = newCombo->prev;
		}

		node_t * garbage = newCombo;
		newCombo = newCombo->prev;
		free(garbage);

	}
	else if (newCombo->next && (
		c_rank0(newCombo->next->combo) == c_rank0(newCombo->combo)
		&& c_rank1(newCombo->next->combo) == c_rank1(newCombo->combo)
		)) {
		newCombo->next->combo = c_mergeCombo(newCombo->combo, newCombo->next->combo);
		newCombo->next->prev = newCombo->prev;

		if (newCombo->prev) {
			newCombo->prev->next = newCombo->next;
		}

		node_t * garbage = newCombo;
		newCombo = newCombo->next;
		free(garbage);
	}

	range.searchPtr = newCombo;
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

	node_t * searchPtr = range.comboList;
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
	for (node_t * node = range.comboList; node != NULL; node = node->next) {
		if (c_type(node->combo) != NO_TYPE) {
			c_printString(node->combo);
		}

		printf(" ");
	}
	printf("\n");
}
