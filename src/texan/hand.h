#pragma once

#include "ll.h"
#include "card.h"

typedef enum {
	STRAIGHT_FLUSH, FOUR_OF_A_KIND, FULL_HOUSE, FLUSH, STRAIGHT, THREE_OF_A_KIND, TWO_PAIR, ONE_PAIR, HIGH_CARD, NULL_STRENGTH
} strength_t;

typedef struct {
	card_t cards[2];
	unsigned strength;
} hand_t;

hand_t h_newNullHand();
hand_t h_newHand(card_t card0, card_t card1);
unsigned h_isNull(hand_t hand);

strength_t h_strength(hand_t hand);

hand_t h_calcStrength(hand_t hand, card_t * community, unsigned nCards);

data h_encode(hand_t hand);
void h_decode(data _data, void* _hand);

signed h_compare(hand_t a, hand_t b);
signed h_compareData(data _a, data _b);

void h_toString(hand_t hand, char * str);

unsigned h_contains(hand_t x, hand_t y);
unsigned h_containsCard(hand_t hand, card_t card);
