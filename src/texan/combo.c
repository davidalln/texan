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

combo_t c_newBlankCombo(rank_t rank0, rank_t rank1, combo_type_t type) {
	combo_t combo = c_newNullCombo();

	if (rank0 < NULL_RANK && rank1 < NULL_RANK && 
		((type != PAIR && rank0 != rank1) || (type == PAIR && rank0 == rank1))
	) {
		combo.type = type;
		combo.ranks[0] = (rank0 < rank1) ? rank0 : rank1;
		combo.ranks[1] = (rank0 < rank1) ? rank1 : rank0;
	}

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
	ll_deleteList(combo.hands);
}

unsigned c_isNull(combo_t combo) {
	return (combo.type == NULL_COMBO || ll_isNull(combo.hands));
}

data c_encode(combo_t combo) {
	data activeHands = 0;

	if (!c_isNull(combo)) {
		ll_t start = combo.hands;
		do {
			hand_t hand;
			ll_get(combo.hands, &hand);

			activeHands |= (1 << (hand.cards[0].suit * 4 + hand.cards[1].suit));

			combo.hands = ll_next(combo.hands);	
		} while (start.ptr != combo.hands.ptr);
	}

	return (combo.ranks[0] << 24) | (combo.ranks[1] << 20) | (combo.type << 16) | activeHands;
}

void c_decode(data _data, void* _combo) {
	combo_t *combo = (combo_t *)(_combo);

	combo->ranks[0] = _data >> 24 & 0b1111;
	combo->ranks[1] = _data >> 20 & 0b1111;
	combo->type = _data >> 16 & 0b1111;

	unsigned activeHands = _data & 0b1111111111111111;
	for (int i = 0; i < 16; i++) {
		unsigned handActive = activeHands & (1 << 15 - i);
		if (handActive) {
			suit_t suit0 = i % 4;
			suit_t suit1 = i / 4;	

			card_t card0 = d_newCard(combo->ranks[0], suit0);
			card_t card1 = d_newCard(combo->ranks[1], suit1);

			hand_t hand = h_newHand(card0, card1);
			combo->hands = ll_add(combo->hands, h_encode(hand));
		}
	}	
}

unsigned c_addHand(combo_t * combo, hand_t hand) {
	combo->hands = ll_add(combo->hands, h_encode(hand));
	return combo->hands.length;
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
						combo.hands = ll_delete(combo.hands);
					}
				}
			}
		}
	}

	return combo;
}

signed c_compare(combo_t a, combo_t b) {
	return c_compareData(c_encode(a), c_encode(b));
}

signed c_compareRankAndType(combo_t a, combo_t b) {
	if (a.ranks[0] < b.ranks[0]) {
		return 1;
	}
	else if (a.ranks[0] == b.ranks[0]) {
		if (a.ranks[1] < b.ranks[1]) {
			return 1;
		}
		else if (a.ranks[1] == b.ranks[1]) {
			if (a.type < b.type) return 1;
			else if (a.type == b.type) return 0;
			else return -1;
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
}

signed c_compareData(data _a, data _b) {
	if (_a < _b) return 1;
	else if (_a > _b) return -1;
	else return 0;
}

void c_toString(combo_t combo, char * str) {
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

