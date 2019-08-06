#pragma once

#include "deck.h"
#include "hand.h"
#include "range.h"
#include "card.h"

float q_calculateHandVsHandEquity(hand_t x, hand_t y, deck_t * deck, card_t * community, unsigned nCommunity);
float q_calculateHandVsRangeEquity(hand_t h, range_t * range, deck_t * deck, card_t * community, unsigned nCommunity);