#include <stdio.h>
#include <stdlib.h>

#include "hand.h"

hand_t h_newNullHand() {
	hand_t hand;
	hand.cards[0] = d_newNullCard();
	hand.cards[1] = d_newNullCard();
	return hand;
}

void h_decode(data _data, void* _hand) {
	rank_t rank0 = _data >> 12 & 0b1111;
	rank_t rank1 = _data >> 8 & 0b1111;
	suit_t suit0 = _data >> 4 & 0b1111;
	suit_t suit1 = _data & 0b1111;

	card_t card0 = d_newCard(rank0, suit0);
	card_t card1 = d_newCard(rank1, suit1);

	hand_t *hand = (hand_t *)(_hand);
	*hand = h_newHand(card0, card1);
}

data h_encode(hand_t hand) {
	card_t card0 = hand.cards[0];
	card_t card1 = hand.cards[1];
	return (card0.rank << 12) | (card1.rank << 8) | (card0.suit << 4) | (card1.suit);
}

signed h_compare(hand_t a, hand_t b) {
	return h_compareData(h_encode(a), h_encode(b));
}

signed h_compareData(data _a, data _b) {
	if (_a < _b) return 1;
	else if (_a > _b) return -1;
	else return 0;
}

hand_t h_newHand(card_t card0, card_t card1) {
	hand_t hand = h_newNullHand();

	if (card0.rank == card1.rank) {
		if (card0.suit < card1.suit) {
			hand.cards[0] = card0;
			hand.cards[1] = card1;
		} else if (card0.suit > card1.suit) {
			hand.cards[0] = card1;
			hand.cards[1] = card0;
		}
	} else if (card0.rank < card1.rank) {
		hand.cards[0] = card0;
		hand.cards[1] = card1;
	} else {
		hand.cards[0] = card1;
		hand.cards[1] = card0;
	}

	return hand;
}

unsigned h_isNull(hand_t hand) {
	return d_isNull(hand.cards[0]) && d_isNull(hand.cards[1]);
}

void h_toString(hand_t hand, char * str) {
	char card0[3];
	char card1[3];

	d_toString(hand.cards[0], card0);
	d_toString(hand.cards[1], card1);

	str[0] = card0[0];
	str[1] = card0[1];
	str[2] = card1[0];
	str[3] = card1[1];
	str[4] = '\0';
}

