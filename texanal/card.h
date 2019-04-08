#pragma once

#include "ll.h"

typedef enum {
	ACE, KING, QUEEN, JACK, TEN, NINE, EIGHT, SEVEN, SIX, FIVE, FOUR, THREE, TWO, NULL_RANK
} rank_t;

typedef enum {
	SPADE, HEART, DIAMOND, CLUB, NULL_SUIT
} suit_t;

typedef struct {
	rank_t rank;
	suit_t suit;
} card_t;

card_t d_newNullCard();
card_t d_newCard(rank_t rank, suit_t suit);
unsigned d_isNull(card_t card);

signed d_compare(card_t a, card_t b);
signed d_compareRanks(card_t a, card_t b);
signed d_compareSuits(card_t a, card_t b);

data d_encode(card_t card);

void d_toString(card_t card, char * str);
