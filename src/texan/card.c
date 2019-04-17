#include <stdio.h>
#include <stdlib.h>

#include "card.h"

card_t d_newNullCard() {
	return d_newCard(NULL_RANK, NULL_SUIT);
}

card_t d_newCard(rank_t rank, suit_t suit) {
	card_t card;
	card.rank = rank;
	card.suit = suit;
	return card;
}

unsigned d_isNull(card_t card) {
	return (card.rank == NULL_RANK && card.suit == NULL_SUIT);
}

signed d_compare(card_t a, card_t b) {
	signed cmp = d_compareRanks(a, b);
	if (cmp) return cmp;
	else return d_compareSuits(a, b);
}
signed d_compareRanks(card_t a, card_t b) {
	if (a.rank == b.rank) return 0;
	else return (a.rank < b.rank) ? 1 : -1;
}
signed d_compareSuits(card_t a, card_t b) {
	if (a.suit == b.suit) return 0;
	else return (a.suit < b.suit) ? 1 : -1;
}

data d_encode(card_t card) {
	return (card.rank << 4 | card.suit);
}

void d_toString(card_t d, char * str) {
	switch (d.rank) {
		case ACE: str[0] = 'A'; break;
		case KING: str[0] = 'K'; break;
		case QUEEN: str[0] = 'Q'; break;
		case JACK: str[0] = 'J'; break;
		case TEN: str[0] = 'T'; break;
		case NINE: str[0] = '9'; break;
		case EIGHT: str[0] = '8'; break;
		case SEVEN: str[0] = '7'; break;
		case SIX: str[0] = '6'; break;
		case FIVE: str[0] = '5'; break;
		case FOUR: str[0] = '4'; break;
		case THREE: str[0] = '3'; break;
		case TWO: str[0] = '2'; break;
		default: str[0] = '_'; break;
	}

	switch (d.suit) {
		case SPADE: str[1] = 's'; break;
		case HEART: str[1] = 'h'; break;
		case DIAMOND: str[1] = 'd'; break;
		case CLUB: str[1] = 'c'; break;
		default: str[1] = '_'; break;
	}
		
	str[2] = '\0';
}

