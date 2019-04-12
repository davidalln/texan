#include <stdio.h>
#include <stdlib.h>

#include "ll.h"
#include "range.h"

range_t r_newRange() {
	range_t range;

	range.combos = ll_newList();
	range.combos.decode = c_decode;
	range.combos.compare = c_compareData;
	return range;
}

unsigned r_isNull(range_t range) {
	return ll_isNull(range.combos);
}

void r_deleteRange(range_t range) {
	ll_deleteList(range.combos);
}

range_t r_addCombo(range_t range, combo_t combo) {
	range.combos = ll_add(range.combos, c_encode(combo));
	return range;
}
