#include <stdio.h>
#include "parser.card.h"

unsigned dp_parseCardString(const char * string, size_t BUFFER_SIZE, card_t * cards)
{
	unsigned string_idx = 0;
	unsigned card_idx = 0;
	while (string_idx < BUFFER_SIZE && string[string_idx] != '\0') {
		char c0 = string[string_idx];
		char c1 = string[string_idx + 1];
		unsigned atCard_idx = card_idx;
		
		switch (c0) {
		case 'A':
			switch (c1) {
			case 's': cards[card_idx++] = d_newCard(ACE, SPADE);		break;
			case 'h': cards[card_idx++] = d_newCard(ACE, HEART);		break;
			case 'd': cards[card_idx++] = d_newCard(ACE, DIAMOND);		break;
			case 'c': cards[card_idx++] = d_newCard(ACE, CLUB);			break;
			}
			break;
		case 'K':
			switch (c1) {
			case 's': cards[card_idx++] = d_newCard(KING, SPADE);		break;
			case 'h': cards[card_idx++] = d_newCard(KING, HEART);		break;
			case 'd': cards[card_idx++] = d_newCard(KING, DIAMOND);		break;
			case 'c': cards[card_idx++] = d_newCard(KING, CLUB);			break;
			}
			break;
		case 'Q':
			switch (c1) {
			case 's': cards[card_idx++] = d_newCard(QUEEN, SPADE);		break;
			case 'h': cards[card_idx++] = d_newCard(QUEEN, HEART);		break;
			case 'd': cards[card_idx++] = d_newCard(QUEEN, DIAMOND);		break;
			case 'c': cards[card_idx++] = d_newCard(QUEEN, CLUB);			break;
			}
			break;
		case 'J':
			switch (c1) {
			case 's': cards[card_idx++] = d_newCard(JACK, SPADE);		break;
			case 'h': cards[card_idx++] = d_newCard(JACK, HEART);		break;
			case 'd': cards[card_idx++] = d_newCard(JACK, DIAMOND);		break;
			case 'c': cards[card_idx++] = d_newCard(JACK, CLUB);			break;
			}
			break;
		case 'T':
			switch (c1) {
			case 's': cards[card_idx++] = d_newCard(TEN, SPADE);		break;
			case 'h': cards[card_idx++] = d_newCard(TEN, HEART);		break;
			case 'd': cards[card_idx++] = d_newCard(TEN, DIAMOND);		break;
			case 'c': cards[card_idx++] = d_newCard(TEN, CLUB);			break;
			}
			break;
		case '9':
			switch (c1) {
			case 's': cards[card_idx++] = d_newCard(NINE, SPADE);		break;
			case 'h': cards[card_idx++] = d_newCard(NINE, HEART);		break;
			case 'd': cards[card_idx++] = d_newCard(NINE, DIAMOND);		break;
			case 'c': cards[card_idx++] = d_newCard(NINE, CLUB);			break;
			}
			break;
		case '8':
			switch (c1) {
			case 's': cards[card_idx++] = d_newCard(EIGHT, SPADE);		break;
			case 'h': cards[card_idx++] = d_newCard(EIGHT, HEART);		break;
			case 'd': cards[card_idx++] = d_newCard(EIGHT, DIAMOND);		break;
			case 'c': cards[card_idx++] = d_newCard(EIGHT, CLUB);			break;
			}
			break;
		case '7':
			switch (c1) {
			case 's': cards[card_idx++] = d_newCard(SEVEN, SPADE);		break;
			case 'h': cards[card_idx++] = d_newCard(SEVEN, HEART);		break;
			case 'd': cards[card_idx++] = d_newCard(SEVEN, DIAMOND);		break;
			case 'c': cards[card_idx++] = d_newCard(SEVEN, CLUB);			break;
			}
			break;
		case '6':
			switch (c1) {
			case 's': cards[card_idx++] = d_newCard(SIX, SPADE);		break;
			case 'h': cards[card_idx++] = d_newCard(SIX, HEART);		break;
			case 'd': cards[card_idx++] = d_newCard(SIX, DIAMOND);		break;
			case 'c': cards[card_idx++] = d_newCard(SIX, CLUB);			break;
			}
			break;
		case '5':
			switch (c1) {
			case 's': cards[card_idx++] = d_newCard(FIVE, SPADE);		break;
			case 'h': cards[card_idx++] = d_newCard(FIVE, HEART);		break;
			case 'd': cards[card_idx++] = d_newCard(FIVE, DIAMOND);		break;
			case 'c': cards[card_idx++] = d_newCard(FIVE, CLUB);			break;
			}
			break;
		case '4':
			switch (c1) {
			case 's': cards[card_idx++] = d_newCard(FOUR, SPADE);		break;
			case 'h': cards[card_idx++] = d_newCard(FOUR, HEART);		break;
			case 'd': cards[card_idx++] = d_newCard(FOUR, DIAMOND);		break;
			case 'c': cards[card_idx++] = d_newCard(FOUR, CLUB);			break;
			}
			break;
		case '3':
			switch (c1) {
			case 's': cards[card_idx++] = d_newCard(THREE, SPADE);		break;
			case 'h': cards[card_idx++] = d_newCard(THREE, HEART);		break;
			case 'd': cards[card_idx++] = d_newCard(THREE, DIAMOND);		break;
			case 'c': cards[card_idx++] = d_newCard(THREE, CLUB);			break;
			}
			break;
		case '2':
			switch (c1) {
			case 's': cards[card_idx++] = d_newCard(TWO, SPADE);		break;
			case 'h': cards[card_idx++] = d_newCard(TWO, HEART);		break;
			case 'd': cards[card_idx++] = d_newCard(TWO, DIAMOND);		break;
			case 'c': cards[card_idx++] = d_newCard(TWO, CLUB);			break;
			}
			break;
		}
	
		if (atCard_idx == card_idx) {
			fprintf("dp_parseCardString: error parsing string %s\n", string);
			return card_idx;
		}

		string_idx += 2;
	}

	if (card_idx > MAX_CARD_STRING_LENGTH) {
		fprintf("dp_parseCardString: too many cards in string %s (max: %d)\n", string, MAX_CARD_STRING_LENGTH);
		return MAX_CARD_STRING_LENGTH;
	}

	return card_idx;
}
