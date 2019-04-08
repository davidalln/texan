#include "parseRange.h"

range_t u_parseRange(char * buffer) {
	range_t range = r_newRange();

	combo_t combo0 = c_newBlankCombo();
	combo_t combo1 = c_newBlankCombo();
	rank_t rank0 = NO_RANK;
	rank_t rank1 = NO_RANK;

	for (char *c = buffer; *c != '\0'; c++) {
		switch (*c) {
		case '2':
			if (rank0 == NO_RANK)		rank0 = TWO;
			else if (rank1 == NO_RANK)	rank1 = TWO;
			break;
		case '3':
			if (rank0 == NO_RANK)		rank0 = THREE;
			else if (rank1 == NO_RANK)	rank1 = THREE;
			break;
		case '4':
			if (rank0 == NO_RANK)		rank0 = FOUR;
			else if (rank1 == NO_RANK)	rank1 = FOUR;
			break;
		case '5':
			if (rank0 == NO_RANK)		rank0 = FIVE;
			else if (rank1 == NO_RANK)	rank1 = FIVE;
			break;
		case '6':
			if (rank0 == NO_RANK)		rank0 = SIX;
			else if (rank1 == NO_RANK)	rank1 = SIX;
			break;
		case '7':
			if (rank0 == NO_RANK)		rank0 = SEVEN;
			else if (rank1 == NO_RANK)	rank1 = SEVEN;
			break;
		case '8':
			if (rank0 == NO_RANK)		rank0 = EIGHT;
			else if (rank1 == NO_RANK)	rank1 = EIGHT;
			break;
		case '9':
			if (rank0 == NO_RANK)		rank0 = NINE;
			else if (rank1 == NO_RANK)	rank1 = NINE;
			break;
		case 'T':
			if (rank0 == NO_RANK)		rank0 = TEN;
			else if (rank1 == NO_RANK)	rank1 = TEN;
			break;
		case 'J':
			if (rank0 == NO_RANK)		rank0 = JACK;
			else if (rank1 == NO_RANK)	rank1 = JACK;
			break;
		case 'Q':
			if (rank0 == NO_RANK)		rank0 = QUEEN;
			else if (rank1 == NO_RANK)	rank1 = QUEEN;
			break;
		case 'K':
			if (rank0 == NO_RANK)		rank0 = KING;
			else if (rank1 == NO_RANK)	rank1 = KING;
			break;
		case 'A':
			if (rank0 == NO_RANK)		rank0 = ACE;
			else if (rank1 == NO_RANK)	rank1 = ACE;
			break;

		case 's':
			if (rank0 > rank1) {
				rank_t tmp = rank1;
				rank1 = rank0;
				rank0 = tmp;
			}
			break;

		case 'o':
			if (rank0 < rank1) {
				rank_t tmp = rank1;
				rank1 = rank0;
				rank0 = tmp;
			}
			break;

		case '+':
			combo1 = c_newCombo(rank0, rank1);
			switch (c_type(combo1)) {
			case PAIR:		combo0 = c_newCombo(ACE, ACE);			break;
			case SUITED:	combo0 = c_newCombo(rank0, rank0 + 1);	break;
			case OFFSUIT:	combo0 = c_newCombo(rank1 + 1, rank1);	break;
			}
			break;

		case '-':
			if (rank0 != NO_RANK && rank1 != NO_RANK) {
				combo0 = c_newCombo(rank0, rank1);
				rank0 = NO_RANK;
				rank1 = NO_RANK;
			}
			break;

		case ',': case ' ':
			if (rank0 != NO_RANK && rank1 != NO_RANK) {
				if (c_type(combo0) == NO_TYPE) {
					combo0 = c_newCombo(rank0, rank1);
				}
				else if (c_type(combo1) == NO_TYPE) {
					combo1 = c_newCombo(rank0, rank1);
					if (c_rank0(combo0) > c_rank0(combo1) || c_rank1(combo0) > c_rank1(combo1)) {
						combo_t tmp = combo1;
						combo1 = combo0;
						combo0 = tmp;
					}
				}

				if (c_type(combo1) == NO_TYPE) {
					range = addComboToRange(range, combo0);
				}
				else {
					for (int i = 0; i <= c_rank1(combo1) - c_rank1(combo0); i++) {
						combo_t newCombo;
						switch (c_type(combo0)) {
						case PAIR:
							newCombo = c_newCombo(c_rank0(combo0) + i, c_rank1(combo0) + i);
							break;
						case SUITED:
							newCombo = c_newCombo(c_rank0(combo0), c_rank1(combo0) + i);
							break;
						case OFFSUIT:
							newCombo = c_newCombo(c_rank1(combo0) + i, c_rank0(combo0));
							break;
						}

						range = addComboToRange(range, newCombo);
					}
				}

				combo0 = c_newBlankCombo();
				combo1 = c_newBlankCombo();
				rank0 = NO_RANK;
				rank1 = NO_RANK;


			}
			break;
		}
	}

	r_printCombos(range);

	return range;
}
