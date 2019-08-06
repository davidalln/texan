#include "deck.h"

// https://stackoverflow.com/a/2999130
// Author: Jerry Coffin
// Pub date: 6/8/10
unsigned rand_lim(int limit) {
	/* return a random number between 0 and limit inclusive.
	 */

	unsigned divisor = RAND_MAX / (limit + 1);
	unsigned retval;

	do {
		retval = rand() / divisor;
	} while (retval > limit);

	return retval;
}

deck_t * k_newDeck() {
	deck_t * deck = (deck_t *)malloc(sizeof(deck_t));

	// https://math.stackexchange.com/a/2522199
	for (int i = 0; i < 52; i++) {
		deck->shuffle[i] = i;
	}

	for (int k = 51; k >= 0; k--) {
		int r = rand_lim(k);
		char tmp = (char)deck->shuffle[r];
		deck->shuffle[r] = deck->shuffle[k];
		deck->shuffle[k] = tmp;
	}

	deck->atCard = 0;

	return deck;
}

void k_deleteDeck(deck_t * deck)
{
	free(deck);
}

deck_t * k_copyDeck(deck_t * deck) {
	deck_t * copy = (deck_t*)malloc(sizeof(deck_t));
	for (int i = 0; i < 52; i++) {
		copy->shuffle[i] = deck->shuffle[i];
	}
	copy->atCard = deck->atCard;
	return copy;
}

unsigned k_cardsRemaining(deck_t * deck) {
	return 52 - deck->atCard;
}

card_t k_deal(deck_t * deck) {
	if (deck->atCard >= 52) {
		return d_newNullCard();
	}

	unsigned s = deck->shuffle[deck->atCard++];
	rank_t rank = (rank_t)(s / 4);
	suit_t suit = (rank_t)(s % 4);

	return d_newCard(rank, suit);
}

void k_removeCards(deck_t * deck, card_t * cards, unsigned nCards) {
	for (int c = 0; c < nCards; c++) {
		card_t card = cards[c];
		char shuffle_idx = card.rank * 4 + card.suit;
		for (int i = 0; i < 52; i++) {
			if (deck->shuffle[i] == shuffle_idx) {
				char tmp = deck->shuffle[i];
				deck->shuffle[i] = deck->shuffle[deck->atCard];
				deck->shuffle[deck->atCard++] = tmp;
			}
		}
	}
}