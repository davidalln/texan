#include <stdio.h>
#include <stdlib.h>

#include "hand.h"

const unsigned STRENGTH_GAP = 1 << 20;
const unsigned KICKER_GAP[5] = {
	1 << 16,
	1 << 12,
	1 << 8,
	1 << 4,
	1
};

hand_t h_newNullHand() {
	hand_t hand;
	hand.cards[0] = d_newNullCard();
	hand.cards[1] = d_newNullCard();

	hand.strength = NULL_STRENGTH * STRENGTH_GAP;
	return hand;
}

strength_t h_strength(hand_t hand) {
	return hand.strength / STRENGTH_GAP;
}

hand_t h_calcStrength(hand_t hand, card_t * community, unsigned nCards) {
	for (int i = 0; i < nCards; i++) {
		if (h_containsCard(hand, community[i])) {
			char c_str[10];
			d_toString(community[i], c_str);
			fprintf(stderr, "h(%s)\n", c_str);
		}
	}

	card_t allCards[8] = {
		hand.cards[0], hand.cards[1],
		d_newNullCard(), d_newNullCard(),
		d_newNullCard(), d_newNullCard(),
		d_newNullCard(), d_newNullCard()
	};

	for (int i = 0; i < nCards; i++) {
		allCards[i + 2] = community[i];
	}

	unsigned nAllCards = nCards + 2;

	// sort all cards by rank
	for (int i = 0; i < nAllCards; i++) {
		card_t x = allCards[i];
		unsigned idx = i;

		for (int j = i + 1; j < nAllCards; j++) {
			if (d_compare(x, allCards[j]) < 0) {
				x = allCards[j];
				idx = j;
			}
		}

		card_t tmp = allCards[i];
		allCards[i] = x;
		allCards[idx] = tmp;
	}

	unsigned hasAce = (allCards[0].rank == ACE);

	hand.strength = STRENGTH_GAP * HIGH_CARD + (
		KICKER_GAP[0] * allCards[0].rank
		+ KICKER_GAP[1] * allCards[1].rank
		+ KICKER_GAP[2] * allCards[2].rank
		+ KICKER_GAP[3] * allCards[3].rank
		+ KICKER_GAP[4] * allCards[4].rank
	);

	// scan for same ranks
	unsigned pairRun = 0;
	unsigned straightRun = 1;
	unsigned straightFlushRun = 1;
	unsigned flushRun[4] = { 0, 0, 0, 0 };

	card_t lastPair = d_newNullCard();

	card_t lastSeen = allCards[0];
	flushRun[lastSeen.suit]++;

	for (int i = 1; i < nAllCards + 1; i++) {
		if (!d_isNull(allCards[i]) && ++flushRun[allCards[i].suit] >= 5 && h_strength(hand) > FLUSH) {
			suit_t flushSuit = allCards[i].suit;
			card_t kickers[5] = { d_newNullCard(), d_newNullCard(), d_newNullCard(), d_newNullCard(), d_newNullCard() };
			unsigned kicker_idx = 0;
			for (int z = 0; z < nAllCards && kicker_idx < 5; z++) {
				if (allCards[z].suit == flushSuit) {
					kickers[kicker_idx++] = allCards[z];
				}
			}

			hand.strength = FLUSH * STRENGTH_GAP + (
				KICKER_GAP[0] * kickers[0].rank
				+ KICKER_GAP[1] * kickers[1].rank
				+ KICKER_GAP[2] * kickers[2].rank
				+ KICKER_GAP[3] * kickers[3].rank
				+ KICKER_GAP[4] * kickers[4].rank
			);

		}

		unsigned deltaRank = allCards[i].rank - lastSeen.rank;
		if (deltaRank == 0) {
			pairRun++;
		}
		else {
			if (deltaRank == 1 && allCards[i].rank != NULL_RANK) {
				straightRun++;
				if (allCards[i].suit == lastSeen.suit || (pairRun == 1 && allCards[i].suit == allCards[i - 2].suit) || (pairRun == 2 && allCards[i].suit == allCards[i - 3].suit)) {
					straightFlushRun++;
				}
				else {
					straightFlushRun = 1;
				}
			}
			else {
				straightRun = 1;
				straightFlushRun = 1;
			}

			if (pairRun > 0) {
				rank_t pairRank = lastSeen.rank;
				card_t kickers[3] = { d_newNullCard(), d_newNullCard(), d_newNullCard() };
				unsigned kicker_idx = 0;

				if (pairRun == 1) {
					switch (h_strength(hand)) {
					case HIGH_CARD: case NULL_STRENGTH:
						lastPair = lastSeen;

						for (int z = 0; z < nAllCards && kicker_idx < 3; z++) {
							if (allCards[z].rank != pairRank) {
								kickers[kicker_idx++] = allCards[z];
							}
						}

						hand.strength = ONE_PAIR * STRENGTH_GAP + (
							KICKER_GAP[0] * pairRank
							+ KICKER_GAP[1] * pairRank
							+ KICKER_GAP[2] * kickers[0].rank
							+ KICKER_GAP[3] * kickers[1].rank
							+ KICKER_GAP[4] * kickers[2].rank
						);
						break;
					case ONE_PAIR:
						for (int z = 0; z < nAllCards && kicker_idx < 1; z++) {
							if (allCards[z].rank != pairRank && allCards[z].rank != lastPair.rank) {
								kickers[kicker_idx++] = allCards[z];
							}
						}

						hand.strength = TWO_PAIR * STRENGTH_GAP + (
							KICKER_GAP[0] * lastPair.rank
							+ KICKER_GAP[1] * lastPair.rank
							+ KICKER_GAP[2] * pairRank
							+ KICKER_GAP[3] * pairRank
							+ KICKER_GAP[4] * kickers[0].rank
						);
						break;
					case THREE_OF_A_KIND:
						hand.strength = FULL_HOUSE * STRENGTH_GAP + (
							KICKER_GAP[0] * lastPair.rank
							+ KICKER_GAP[1] * lastPair.rank
							+ KICKER_GAP[2] * lastPair.rank
							+ KICKER_GAP[3] * pairRank
							+ KICKER_GAP[4] * pairRank
						);
						break;
					default:
						break;
					}
				}
				else if (pairRun == 2) {
					switch (h_strength(hand)) {
					case HIGH_CARD: case NULL_STRENGTH:
						lastPair = lastSeen;

						for (int z = 0; z < nAllCards && kicker_idx < 2; z++) {
							if (allCards[z].rank != pairRank) {
								kickers[kicker_idx++] = allCards[z];
							}
						}
						hand.strength = THREE_OF_A_KIND * STRENGTH_GAP + (
							KICKER_GAP[0] * pairRank
							+ KICKER_GAP[1] * pairRank
							+ KICKER_GAP[2] * pairRank
							+ KICKER_GAP[3] * kickers[0].rank
							+ KICKER_GAP[4] * kickers[1].rank
						);
						break;
					case ONE_PAIR: case TWO_PAIR:
						hand.strength = FULL_HOUSE * STRENGTH_GAP + (
							KICKER_GAP[0] * pairRank
							+ KICKER_GAP[1] * pairRank
							+ KICKER_GAP[2] * pairRank
							+ KICKER_GAP[3] * lastPair.rank
							+ KICKER_GAP[4] * lastPair.rank
						);
						break;
					default:
						break;
					}
				}
				else {
					for (int z = 0; z < nAllCards && kicker_idx < 1; z++) {
						if (allCards[z].rank != pairRank && allCards[z].rank != lastPair.rank) {
							kickers[kicker_idx++] = allCards[z];
						}
					}

					hand.strength = FOUR_OF_A_KIND * STRENGTH_GAP + (
						KICKER_GAP[0] * pairRank
						+ KICKER_GAP[1] * pairRank
						+ KICKER_GAP[2] * pairRank
						+ KICKER_GAP[3] * pairRank
						+ KICKER_GAP[4] * kickers[0].rank
					);
				}

				pairRun = 0;
			}

			if (straightRun == 4) {
				if (hasAce && allCards[i].rank == TWO) {
					straightRun++;
					// TODO: straight flush run?
				}
			}
			if (straightRun >= 5) {
				if (straightFlushRun >= 5 && h_strength(hand) > STRAIGHT_FLUSH) {
					hand.strength = STRAIGHT_FLUSH * STRENGTH_GAP + (
						KICKER_GAP[0] * allCards[i - 4].rank
						+ KICKER_GAP[1] * allCards[i - 3].rank
						+ KICKER_GAP[2] * allCards[i - 2].rank
						+ KICKER_GAP[3] * allCards[i - 1].rank
						+ KICKER_GAP[4] * allCards[i].rank
					);
				}
				else if (h_strength(hand) > STRAIGHT) {
					hand.strength = STRAIGHT * STRENGTH_GAP + (
						KICKER_GAP[0] * allCards[i - 4].rank
						+ KICKER_GAP[1] * allCards[i - 3].rank
						+ KICKER_GAP[2] * allCards[i - 2].rank
						+ KICKER_GAP[3] * allCards[i - 1].rank
						+ KICKER_GAP[4] * allCards[i].rank
					);
				}
			}
		}

		lastSeen = allCards[i];
	}

	return hand;
}

void h_decode(data _data, void* _hand) {
	rank_t rank0 = _data >> 12 & 0b1111;
	rank_t rank1 = _data >> 8 & 0b1111;
	suit_t suit0 = _data >> 4 & 0b1111;
	suit_t suit1 = _data & 0b1111;

	card_t card0 = d_newCard(rank0, suit0);
	card_t card1 = d_newCard(rank1, suit1);

	hand_t *hand = (hand_t *)(_hand);
	*hand = h_newHand(card0, card1);
}

data h_encode(hand_t hand) {
	card_t card0 = hand.cards[0];
	card_t card1 = hand.cards[1];
	return (card0.rank << 12) | (card1.rank << 8) | (card0.suit << 4) | (card1.suit);
}

signed h_compare(hand_t a, hand_t b) {
	return h_compareData(h_encode(a), h_encode(b));
}

signed h_compareData(data _a, data _b) {
	if (_a < _b) return 1;
	else if (_a > _b) return -1;
	else return 0;
}

hand_t h_newHand(card_t card0, card_t card1) {
	hand_t hand = h_newNullHand();

	if (card0.rank == card1.rank) {
		if (card0.suit < card1.suit) {
			hand.cards[0] = card0;
			hand.cards[1] = card1;
		} else if (card0.suit > card1.suit) {
			hand.cards[0] = card1;
			hand.cards[1] = card0;
		}
	} else if (card0.rank < card1.rank) {
		hand.cards[0] = card0;
		hand.cards[1] = card1;
	} else {
		hand.cards[0] = card1;
		hand.cards[1] = card0;
	}

	return hand;
}

unsigned h_isNull(hand_t hand) {
	return d_isNull(hand.cards[0]) && d_isNull(hand.cards[1]);
}

unsigned h_contains(hand_t x, hand_t y) {
	return h_containsCard(x, y.cards[0]) || h_containsCard(x, y.cards[1]);
}

unsigned h_containsCard(hand_t hand, card_t card) {
	return (d_compare(hand.cards[0], card) == 0 || d_compare(hand.cards[1], card) == 0);
}

void h_toString(hand_t hand, char * str) {
	char card0[3];
	char card1[3];

	d_toString(hand.cards[0], card0);
	d_toString(hand.cards[1], card1);

	str[0] = card0[0];
	str[1] = card0[1];
	str[2] = card1[0];
	str[3] = card1[1];
	str[4] = '\0';
}

