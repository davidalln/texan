#pragma once

typedef unsigned char card_t;

typedef enum {
	ACE, KING, QUEEN, JACK, TEN, NINE, EIGHT, SEVEN, SIX, FIVE, FOUR, THREE, TWO, NO_RANK
} rank_t;

typedef enum {
	SPADE, HEART, DIAMOND, CLUB, NO_SUIT
} suit_t;

typedef struct {
	card_t card0;
	card_t card1;
} hand_t;

card_t h_newBlankCard();
card_t h_newCard(rank_t rank, suit_t suit);
hand_t h_newHand(rank_t rank0, suit_t suit0, rank_t rank1, suit_t suit1);

rank_t h_cardRank(card_t card);
suit_t h_cardSuit(card_t card);

rank_t h_rank0(hand_t hand);
rank_t h_rank1(hand_t hand);

suit_t h_suit0(hand_t hand);
suit_t h_suit1(hand_t hand);

unsigned h_hasCard(hand_t hand, card_t card);
unsigned h_hasCardRank(hand_t hand, card_t card);

void h_cardPrintString(card_t card);
void h_printString(hand_t hand);
