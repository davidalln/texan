#include <stdio.h>
#include "equity.h"

float q_calculateHandVsHandEquity(hand_t x, hand_t y, deck_t * deck, card_t * community, unsigned nCommunity) {
	if (nCommunity >= 5) {

		if (h_contains(x, y)) {
			char str0[10];
			char str1[10];
			h_toString(x, str0);
			h_toString(y, str1);
			fprintf(stderr, "q_calculateHandVsHandEquity: hands share a card (%s, %s)\n", str0, str1);
		}
		
		for (int i = 0; i < nCommunity; i++) {
			if (h_containsCard(x, community[i]) || h_containsCard(y, community[i])) {
				fprintf(stderr, "q_calculateHandVsHandEquity: found a community card in the hand\n");
			}
		}
		x = h_calcStrength(x, community, nCommunity);
		y = h_calcStrength(y, community, nCommunity);

		char str0[10];
		d_toString(community[4], str0);

		//printf("\t(%s) %df, %df, %.2f\n", str0, x.strength, y.strength, (x.strength < y.strength) ? 1.f : 0.f);

		if (x.strength == y.strength) return 0.5f;
		else return (x.strength < y.strength) ? 1.f : 0.f;
	}
	else {
		if (nCommunity < 3) {
			fprintf(stderr, "q_calculateHandVsHandEquity: attempted to calculate equity with less than 3 community cards\n");
			return 0.f;
		}

		unsigned deckLength = k_cardsRemaining(deck);

		card_t * cards = malloc(sizeof(card_t) * deckLength);
		for (int i = 0; i < deckLength; i++) {
			cards[i] = k_deal(deck);
		}

		float total = 0.f;
		float constant = 1.f;

		if (nCommunity == 3) {
			constant = 2.f / (deckLength * (deckLength - 1));

			for (int i = 0; i < deckLength; i++) {
				community[3] = cards[i];
				for (int j = i + 1; j < deckLength; j++) {
					community[4] = cards[j];
					total += constant * q_calculateHandVsHandEquity(x, y, deck, community, 5);
				}
			}
		}
		else if (nCommunity == 4) {
			constant = 1.f / deckLength;

			for (int i = 0; i < deckLength; i++) {
				community[4] = cards[i];
				total += constant * q_calculateHandVsHandEquity(x, y, deck, community, 5);
			}
		}

		free(cards);
		return total;
	}
}


float q_calculateHandVsRangeEquity(hand_t h, range_t * range, deck_t * deck, card_t * community, unsigned nCommunity) {
	char hh_str[10];
	h_toString(h, hh_str);

	// ensure that the hand is removed from the range (it will be added back at the end)
	hand_t removedHands[128];
	unsigned nHandsDeleted;
	r_deleteCards_hands(range, h.cards, 2, removedHands, &nHandsDeleted);

	hand_t *hands = r_generateHandArray(range);
	float equity = 0.f;
	unsigned nHands = range->nHands;

	printf("DELETED %d HANDS: ", nHandsDeleted);
	for (int i = 0; i < nHandsDeleted; i++) {
		char strr[10];
		h_toString(removedHands[i], strr);
		printf("%s, ", strr);
	}
	printf(" (%d hands in range)\n", range->nHands);

	printf("BOARD: ");
	for (int i = 0; i < nCommunity; i++) {
		char str0[10];
		d_toString(community[i], str0);
		printf("%s", str0);
	}
	printf("\n\n");

	for (int i = 0; i < nHands; i++) {
		if (h_compare(h, hands[i]) == 0) {
			fprintf(stderr, "q_calculateHandVsRangeEquity: WARNING! comparing hand with a hand in the range (skipping)\n");
			nHands--;
		}
		else {
			card_t communityCopy[5];
			deck_t * deckCopy = k_copyDeck(deck);
			for (int j = 0; j < 5; j++) {
				communityCopy[j] = (j >= nCommunity) ? d_newNullCard() : community[j];
			}
			k_removeCards(deckCopy, hands[i].cards, 2);
			float e = q_calculateHandVsHandEquity(h, hands[i], deckCopy, communityCopy, nCommunity);
			k_deleteDeck(deckCopy);

			char h_str[10];
			h_toString(hands[i], h_str);
			printf("%s vs. %s: %.4f equity\n", hh_str, h_str, e);
			equity += e;
		}
	}

	// put the hands back in the range
	for (int i = 0; i < nHandsDeleted; i++) {
		combo_t combo = c_newComboFromHand(removedHands[i]);
		r_addCombo(range, combo);

		char ssstr[10];
		c_toString(combo, ssstr);
		printf("\tadding back combo %s\n", ssstr);

		c_deleteCombo(combo);
	}

	printf("hand %s has equity %.4f\n", hh_str, (1.f / nHands) * equity);

	return (1.f / (nHands)) * equity;
}