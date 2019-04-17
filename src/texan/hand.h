#pragma once

#include "ll.h"
#include "card.h"

typedef struct {
	card_t cards[2];
} hand_t;

hand_t h_newNullHand();
hand_t h_newHand(card_t card0, card_t card1);
unsigned h_isNull();

data h_encode(hand_t hand);
void h_decode(data _data, void* _hand);

signed h_compare(hand_t a, hand_t b);
signed h_compareData(data _a, data _b);

void h_toString(hand_t hand, char * str);
