#include <stdlib.h>
#include <stdio.h>

#include "hand.h"

card_t h_newBlankCard()
{
	return (NO_RANK << 4) | NO_SUIT;
}

card_t h_newCard(rank_t rank, suit_t suit) {
	return (rank << 4) | suit;
}

hand_t h_newHand(rank_t rank0, suit_t suit0, rank_t rank1, suit_t suit1)
{
	hand_t hand;
	hand.card0 = h_newCard(rank0, suit0);
	hand.card1 = h_newCard(rank1, suit1);
	return hand;
}

rank_t h_cardRank(card_t card)
{
	return card >> 4;
}

suit_t h_cardSuit(card_t card)
{
	return card & 0b1111;
}

rank_t h_rank0(hand_t hand)
{
	return h_cardRank(hand.card0);
}

rank_t h_rank1(hand_t hand)
{
	return  h_cardRank(hand.card1);
}

suit_t h_suit0(hand_t hand)
{
	return h_cardSuit(hand.card0);
}

suit_t h_suit1(hand_t hand)
{
	return h_cardSuit(hand.card1);
}

unsigned h_hasCard(hand_t hand, card_t card)
{
	return (
		(h_rank0(hand) == h_cardRank(card) && h_suit0(hand) == h_cardSuit(card))
		|| (h_rank1(hand) == h_cardRank(card) && h_suit1(hand) == h_cardSuit(card))
		);
}

unsigned h_hasCardRank(hand_t hand, card_t card)
{
	return (
		(h_rank0(hand) == h_cardRank(card))
		|| (h_rank1(hand) == h_cardRank(card))
		);
}

void h_cardPrintString(card_t card) {
	char *string = (char *)malloc(3);

	switch (h_cardRank(card)) {
	case ACE:		string[0] = 'A'; break;
	case KING:		string[0] = 'K'; break;
	case QUEEN:		string[0] = 'Q'; break;
	case JACK:		string[0] = 'J'; break;
	case TEN:		string[0] = 'T'; break;
	case NINE:		string[0] = '9'; break;
	case EIGHT:		string[0] = '8'; break;
	case SEVEN:		string[0] = '7'; break;
	case SIX:		string[0] = '6'; break;
	case FIVE:		string[0] = '5'; break;
	case FOUR:		string[0] = '4'; break;
	case THREE:		string[0] = '3'; break;
	case TWO:		string[0] = '2'; break;
	}

	switch (h_cardSuit(card)) {
	case SPADE:		string[1] = 's'; break;
	case HEART:		string[1] = 'h'; break;
	case DIAMOND:	string[1] = 'd'; break;
	case CLUB:		string[1] = 'c'; break;
	}

	string[2] = '\0';
	printf("%s", string);
}

void h_printString(hand_t hand)
{
	h_cardPrintString(hand.card0);
	h_cardPrintString(hand.card1);
}
