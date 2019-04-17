#include <stdio.h>
#include <stdlib.h>

#include "ll.h"
#include "range.h"

range_t * r_newRange() {
	range_t * range = (range_t *)malloc(sizeof(range_t));

	range->combos = ll_newList();
	range->combos.decode = c_decode;
	range->combos.compare = c_compareData;

	range->name[0] = '\0';

	return range;
}

unsigned r_isNull(range_t range) {
	return ll_isNull(range.combos);
}

void r_deleteRange(range_t * range) {
	ll_deleteList(range->combos);
	free(range);
}

unsigned r_addCombo(range_t * range, combo_t combo) {
	range->combos = ll_add(range->combos, c_encode(combo));
	return range->combos.length;
}

unsigned r_hasCombo(range_t range, combo_t combo) {
	range.combos = ll_search(range.combos, c_encode(combo));

	combo_t searchCombo = c_newNullCombo();
	ll_get(range.combos, &searchCombo);

	signed cmp = (c_compareRankAndType(searchCombo, combo));
	c_deleteCombo(searchCombo);

	return (cmp == 0);
}

range_t r_deleteCards(range_t range, card_t * cards, unsigned nCards) {
	range.combos = ll_head(range.combos);
	unsigned combos_length = range.combos.length;

	for (unsigned i = 0; i < combos_length; i++) {
		combo_t combo = c_newNullCombo();
		ll_get(range.combos, &combo);

		combo = c_deleteCards(combo, cards, nCards);

		if (combo.hands.length == 0) {
			range.combos = ll_delete(range.combos);
		}
		else {
			range.combos.ptr->payload = c_encode(combo);
			range.combos = ll_next(range.combos);
		}
		 
		c_deleteCombo(combo);
	}
	
	return range;
}
