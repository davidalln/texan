#pragma once

#include <stdlib.h>

#include "card.h"

typedef struct {
	char shuffle[52];
	char atCard;
} deck_t;

deck_t * k_newDeck();
void k_deleteDeck(deck_t * deck);
deck_t * k_copyDeck(deck_t * deck);

card_t k_deal(deck_t * deck);
unsigned k_cardsRemaining(deck_t * deck);
void k_removeCards(deck_t * deck, card_t * cards, unsigned nCards);
