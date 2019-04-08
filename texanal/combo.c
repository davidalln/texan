#include <stdio.h>
#include <stdlib.h>

#include "combo.h"

combo_t c_newNullCombo() {
	combo_t combo;

	combo.ranks[0] = NULL_RANK;
	combo.ranks[1] = NULL_RANK;
	combo.type = NULL_COMBO;
	combo.hands = ll_newList();	
	combo.hands.decode = h_decode;
	combo.hands.compare = h_compareData;

	return combo;
}

combo_t c_newCombo(rank_t rank0, rank_t rank1) {
	combo_t combo = c_newNullCombo();

	if (rank0 == rank1) {
		combo.type = PAIR;
		combo.ranks[0] = rank0;
		combo.ranks[1] = rank1;
	} else if (rank0 < rank1) {
		combo.type = SUITED;
		combo.ranks[0] = rank0;
		combo.ranks[1] = rank1;
	} else {
		combo.type = OFFSUIT;
		combo.ranks[0] = rank1;
		combo.ranks[1] = rank0;
	}	

	switch (combo.type) {
	case PAIR:
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i > j) {
					hand_t hand = h_newHand(d_newCard(combo.ranks[0], i), d_newCard(combo.ranks[1], j));
					combo.hands = ll_add(combo.hands, h_encode(hand));
				}
			}
		}
		break;
	case SUITED:
		for (int i = 0; i < 4; i++) {
			hand_t hand = h_newHand(d_newCard(combo.ranks[0], i), d_newCard(combo.ranks[1], i));
			combo.hands = ll_add(combo.hands, h_encode(hand));
		}
		break;
	case OFFSUIT:
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i != j) {
					hand_t hand = h_newHand(d_newCard(combo.ranks[0], i), d_newCard(combo.ranks[1], j));
					combo.hands = ll_add(combo.hands, h_encode(hand));
				}
			}
		}
		break;
	}

	return combo;
}

void c_deleteCombo(combo_t combo) {
	char combostr[10];
	c_toString(combo, combostr);
	ll_deleteList(combo.hands);
}

unsigned c_isNull(combo_t combo) {
	return (combo.type == NULL_COMBO && ll_isNull(combo.hands));
}

combo_t c_deleteCards(combo_t combo, card_t * cards, unsigned nCards) {
	for (int i = 0; i < nCards; i++) {
		card_t card = cards[i];
		
		if (combo.ranks[0] == card.rank || combo.ranks[1] == card.rank) {
			for (int j = 0; j < 8; j++) {
				if (!ll_isNull(combo.hands)) {
					hand_t searchHand;
					if (j < 4) {
						searchHand = h_newHand(card, d_newCard(combo.ranks[1], j));
					} else {
						searchHand = h_newHand(d_newCard(combo.ranks[0], j - 4), card);
					}

					combo.hands = ll_search(combo.hands, h_encode(searchHand));
					
					hand_t foundHand;
					ll_get(combo.hands, &foundHand);

					if (h_compare(searchHand, foundHand) == 0) {
						char handstr[10];
						h_toString(foundHand, handstr);
						combo.hands = ll_delete(combo.hands);
					}
				}
			}
		}
	}

	return combo;
}

signed c_compare(combo_t a, combo_t b) {
	if (a.ranks[0] < b.ranks[0]) {
		return 1;
	} else if (a.ranks[1] < b.ranks[1]) {
		return -1;
	} else {
		return 0;
	}
}

void c_toString(combo_t combo, char * str) {
	if (sizeof(str) < 4) {
		str[0] = '\0';
		fprintf(stderr, "c_toString: buffer not large enough");
	} else {
		char hand_str[5];
		h_toString(h_newHand(d_newCard(combo.ranks[0], SPADE), d_newCard(combo.ranks[1], HEART)), hand_str);

		str[0] = hand_str[0];
		str[1] = hand_str[2];

		switch (combo.type) {
			case PAIR: str[2] = '\0'; break;
			case SUITED: str[2] = 's'; str[3] = '\0'; break;
			case OFFSUIT: str[2] = 'o'; str[3] = '\0'; break;
		}
	}
}

