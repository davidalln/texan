#include "parseFlop.h"

flop_t u_parseFlop(char * buffer) {
	flop_t flop = f_newFlop();

	rank_t rank = NO_RANK;
	suit_t suit = NO_SUIT;
	for (char *c = buffer; *c != '\0'; c++) {
		switch (*c) {
		case '2':	rank = TWO;		break;
		case '3':	rank = THREE;	break;
		case '4':	rank = FOUR;	break;
		case '5':	rank = FIVE;	break;
		case '6':	rank = SIX;		break;
		case '7':	rank = SEVEN;	break;
		case '8':	rank = EIGHT;	break;
		case '9':	rank = NINE;	break;
		case 'T':	rank = TEN;		break;
		case 'J':	rank = JACK;	break;
		case 'Q':	rank = QUEEN;	break;
		case 'K':	rank = KING;	break;
		case 'A':	rank = ACE;		break;
		
		case 's':	if (suit == NO_SUIT) suit = SPADE;	
		case 'h':	if (suit == NO_SUIT) suit = HEART;
		case 'd':	if (suit == NO_SUIT) suit = DIAMOND;
		case 'c':	if (suit == NO_SUIT) suit = CLUB;
			if (rank != NO_RANK) {
				if (h_cardRank(flop.card0) == NO_RANK)			flop.card0 = h_newCard(rank, suit);
				else if (h_cardRank(flop.card1) == NO_RANK)		flop.card1 = h_newCard(rank, suit);
				else if (h_cardRank(flop.card2) == NO_RANK)		flop.card2 = h_newCard(rank, suit);

				rank = NO_RANK;
				suit = NO_SUIT;
			}
			break;
		}
	}
	return flop;
}
