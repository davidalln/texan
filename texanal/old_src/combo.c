#include "combo.h"

#define PAIR_ALL_HANDS_ACTIVE		0b0000100011001110
#define SUITED_ALL_HANDS_ACTIVE		0b1000010000100001
#define OFFSUIT_ALL_HANDS_ACTIVE	0b0111101111011110

#define PAIR_MASK		0b1111011100110001
#define SUITED_MASK		0b0111101111011110
#define OFFSUIT_MASK	0b1000010000100001

#define RANK0_MASK			0b0000111111111111
#define RANK1_MASK			0b1111000011111111
#define RANKS_MASK			0b0000000011111111
#define NHANDSACTIVE_MASK	0b1111111100001111
#define TYPE_MASK			0b1111111111110000

rank_t c_rank0(combo_t combo) {
	return (rank_t)((combo.definition >> 12) & 0b1111);
}

rank_t c_rank1(combo_t combo) {
	return (rank_t)((combo.definition >> 8) & 0b1111);
}

unsigned char c_nHandsActive(combo_t combo) {
	return (unsigned char)((combo.definition >> 3) & 0b11111);
}

comboType_t c_type(combo_t combo) {
	return (comboType_t)(combo.definition & 0b111);
}

combo_t c_setRanks(combo_t combo, rank_t rank0, rank_t rank1) {
	data r0 = ((data)(rank0) << 12) & ~RANK0_MASK;
	data r1 = ((data)(rank1) << 8) & ~RANK1_MASK;
	combo.definition &= ~RANKS_MASK;
	combo.definition |= (r0 | r1);
	return combo;
}

void c_printString(combo_t combo) {
	char *string = (char *) malloc(4);

	switch (c_rank0(combo)) {
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

	switch (c_rank1(combo)) {
	case ACE:		string[1] = 'A'; break;
	case KING:		string[1] = 'K'; break;
	case QUEEN:		string[1] = 'Q'; break;
	case JACK:		string[1] = 'J'; break;
	case TEN:		string[1] = 'T'; break;
	case NINE:		string[1] = '9'; break;
	case EIGHT:		string[1] = '8'; break;
	case SEVEN:		string[1] = '7'; break;
	case SIX:		string[1] = '6'; break;
	case FIVE:		string[1] = '5'; break;
	case FOUR:		string[1] = '4'; break;
	case THREE:		string[1] = '3'; break;
	case TWO:		string[1] = '2'; break;
	}

	switch (c_type(combo)) {
	case PAIR:		string[2] = ' '; break;
	case SUITED:	string[2] = 's'; break;
	case OFFSUIT:	string[2] = 'o'; break;
	case MIXED:		string[2] = 'm'; break;
	}

	string[3] = '\0';

	printf("%s", string);
}

combo_t c_newBlankCombo() {
	combo_t combo;

	rank_t rank0 = NO_RANK;
	rank_t rank1 = NO_RANK;
	comboType_t type = NO_TYPE;
	unsigned char nHandsActive = 0;
	data handsActive = 0;

	combo.definition = 0;
	combo.definition = ((rank0 << 12) & 0b1111000000000000) | ((rank1 << 8) & 0b0000111100000000) | ((nHandsActive << 3) & 0b0000000011111000) | (type & 0b1111);
	combo.handsActive = handsActive;

	return combo;
}

combo_t c_newCombo(rank_t rank0, rank_t rank1)
{
	combo_t combo;

	unsigned char nHandsActive;
	comboType_t type;
	data handsActive;

	if (rank0 == rank1) {
		type = PAIR;
		nHandsActive = 6;
		handsActive = PAIR_ALL_HANDS_ACTIVE;
	}
	else if (rank0 < rank1) {
		type = SUITED;
		nHandsActive = 4;
		handsActive = SUITED_ALL_HANDS_ACTIVE;
	}
	else {
		rank_t temp = rank0;
		rank0 = rank1;
		rank1 = temp;

		type = OFFSUIT;
		nHandsActive = 12;
		handsActive = OFFSUIT_ALL_HANDS_ACTIVE;
	}

	combo.definition = 0;
	combo.definition = ((rank0 << 12) & 0b1111000000000000) | ((rank1 << 8) & 0b0000111100000000) | ((nHandsActive << 3) & 0b0000000011111000) | (type & 0b111);
	combo.handsActive = handsActive;

	return combo;
}

combo_t c_constructCombo(rank_t rank0, rank_t rank1, unsigned char nHandsActive, comboType_t type, data handsActive) {
	combo_t combo = c_newBlankCombo();
	combo.definition = 0;
	combo.definition = ((rank0 << 12) & 0b1111000000000000) | ((rank1 << 8) & 0b0000111100000000) | ((nHandsActive << 3) & 0b0000000011111000) | (type & 0b111);
	combo.handsActive = handsActive;

	return combo;
}

combo_t c_mergeCombo(combo_t combo0, combo_t combo1)
{
	rank_t rank0 = c_rank0(combo0);
	rank_t rank1 = c_rank1(combo0);
	comboType_t type = MIXED;
	unsigned char nHandsActive = c_nHandsActive(combo0) + c_nHandsActive(combo1);

	return c_constructCombo(rank0, rank1, nHandsActive, type, combo0.handsActive | combo1.handsActive);
}

data c_handsOfType(combo_t combo, comboType_t type)
{
	switch (type) {
	case PAIR:
		return combo.handsActive & ~PAIR_MASK;
	case SUITED:
		return combo.handsActive & ~SUITED_MASK;
	case OFFSUIT:
		return combo.handsActive & ~OFFSUIT_MASK;
	default:
		return combo.handsActive;
	}
}

combo_t c_applyDeadCards(combo_t combo0, card_t card0, card_t card1, card_t card2, card_t card3, card_t card4)
{
	unsigned nHandsActive = c_nHandsActive(combo0);
	data handsActive = combo0.handsActive;

	rank_t rank0 = c_rank0(combo0);
	rank_t rank1 = c_rank1(combo0);
	comboType_t type = c_type(combo0);


	for (int i = 0; i < HANDS_PER_COMBO; i++) {
		suit_t suit0 = i % 4;
		suit_t suit1 = i / 4;

		if (handsActive & (1 << (HANDS_PER_COMBO - i - 1))) {
			hand_t hand = h_newHand(rank0, suit0, rank1, suit1);
			if (
				(h_cardRank(card0) != NO_RANK && h_hasCard(hand, card0))
				|| (h_cardRank(card1) != NO_RANK && h_hasCard(hand, card1))
				|| (h_cardRank(card2) != NO_RANK && h_hasCard(hand, card2))
				|| (h_cardRank(card3) != NO_RANK && h_hasCard(hand, card3))
				|| (h_cardRank(card4) != NO_RANK && h_hasCard(hand, card4))
				) {
				printf("found hand ");
				h_printString(hand);
				handsActive = handsActive & ~(1 << (HANDS_PER_COMBO - i - 1));
				nHandsActive = nHandsActive - 1;
				printf(" handsActive: %d nHandsActive: %d", handsActive, nHandsActive);
				printf("\n");
			}
		}
	}

	return c_constructCombo(rank0, rank1, nHandsActive, type, handsActive);
}
