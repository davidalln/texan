#include "texture.h"

texture_t t_newBlankTexture()
{
	texture_t texture;

	texture.cards[0] = h_newBlankCard();
	texture.cards[1] = h_newBlankCard();
	texture.cards[2] = h_newBlankCard();
	texture.cards[3] = h_newBlankCard();
	texture.cards[4] = h_newBlankCard();

	texture.length = 0;

	return texture;
}

void t_applyTextureToRange(texture_t texture, range_t range) {
	r_node_t * comboList = range.comboList;

	// hand + texture length = length of our card array
	unsigned char cards_length = texture.length + 2;
	printf("texture length: %d", cards_length);
	card_t cards[5];
	for (int i = 0; i < 5; i++) {
		cards[i] = h_newBlankCard();
	}

	// loop through each combo in the range
	for (r_node_t * c_node = comboList; c_node != NULL; c_node = c_node->next) {
		combo_t combo = c_node->combo;

		if (c_nHandsActive(combo) > 0) {
			printf("now applying texture to combo ");
			c_printString(combo);
			printf("\n");

			// loop through each active hand in the combo
			for (h_node_t * h_node = c_node->handList; h_node != NULL; h_node = h_node->next) {
				hand_t hole = h_node->hand;
				printf("\t- At hand ");
				h_printString(hole);

				// fill card array sorted by rank
				rank_t hole0Rank = h_rank0(hole);
				rank_t hole1Rank = h_rank1(hole);

				unsigned hole0Position = 0;
				unsigned hole1Position = 0;
				
				for (int i = 0, j = 0; i <= texture.length; i++) {
					if (i < texture.length) {
						if (hole0Rank < h_cardRank(texture.cards[i])) {
							//printf("... placing card0 %d %d", hole0Rank, h_cardRank(texture.cards[i]));
							cards[j] = hole.card0;
							hole0Rank = NO_RANK;		// ensure we only insert the hole card once
							hole0Position = j++;
						}

						if (hole1Rank < h_cardRank(texture.cards[i])) {
							//printf("... placing card1 %d %d", hole1Rank, h_cardRank(texture.cards[i]));
							cards[j] = hole.card1;
							hole1Rank = NO_RANK;
							hole1Position = j++;
						}

						cards[j] = texture.cards[i];
						j++;
					}
					else {
						if (hole0Rank != NO_RANK) {
							//printf("... placing card0 %d at the end", hole0Rank);
							cards[j] = hole.card0;
							hole0Position = j++;
						}
						if (hole1Rank != NO_RANK) {
							//printf("... placing card1 %d at the end", hole1Rank);
							cards[j] = hole.card1;
							hole1Position = j;
						}
					}
				}

				printf(" (");
				for (int i = 0; i < cards_length; i++) {
					h_cardPrintString(cards[i]);
				}
				printf(") ");

				madeHand_t madeHand = t_newBlankMadeHand();
				madeHand.hand[0] = cards[0];
				madeHand.hand[1] = cards[1];
				madeHand.hand[2] = cards[2];
				madeHand.hand[3] = cards[3];
				madeHand.hand[4] = cards[4];

				// overcard analysis
				if (h_cardRank(madeHand.hand[0]) == ACE) {
					madeHand.strength = OC_ACE_HIGH;
				}
				else if (h_cardRank(madeHand.hand[0]) == KING) {
					madeHand.strength = OC_KING_HIGH;
				}
				else if (hole0Position < 2) {
					if (hole1Position < 2) {
						madeHand.strength = OC_TWO_OVERCARDS;
					}
					else {
						madeHand.strength = OC_ONE_OVERCARD;
					}
				}

				// pair analysis
				unsigned char runLength = 0;
				for (int i = 1; i < cards_length; i++) {
					// check if the hole cards are a pocket pair
					unsigned pocketPair = (h_rank0(hole) == h_rank1(hole) && h_rank0(hole) == h_cardRank(cards[i - 1]));

					if (h_cardRank(cards[i]) == h_cardRank(cards[i - 1])) {
						runLength++;
						if (i < cards_length - 1)
							continue;
					}

					if (runLength > 0) {
						// count the previous card amongst the same ranks
						runLength++;

						if (runLength == 2) {
							//printf("... found a pair!");
							if (madeHand.strength <= OP_MAX && madeHand.strength >= OP_MIN) {
								// if we have already found a pair, it must be two pair
								//printf("... make that a two-pair!");

								unsigned boardPair = (!h_hasCardRank(hole, cards[i - 1]) && !h_hasCardRank(hole, cards[i - 2]));

								switch (madeHand.strength) {
								case OP_BOARD_PAIR:
								case OP_BOARD_PAIR_KICKER:
									if (boardPair) {
										madeHand.strength = (madeHand.strength == OP_BOARD_PAIR) ? TP_BOARD : TP_BOARD_KICKER;
									}
									else {
										if (i == 4) {
											madeHand.strength = TP_BOARD_TOP_SIDECARD;
										}
										else if (i == 5) {
											madeHand.strength = TP_BOARD_MIDDLE_SIDECARD;
										}
										else {
											madeHand.strength = TP_BOARD_LOW_SIDECARD;
										}
									}
									break;

								case OP_LOW_PAIR:
									if (boardPair) {
										madeHand.strength = TP_BOARD_LOW_SIDECARD;
									}
									else {
										madeHand.strength = TP_SPLIT;
									}
									break;

								case OP_MIDDLE_PAIR:
									if (boardPair) {
										madeHand.strength = TP_BOARD_MIDDLE_SIDECARD;
									}
									else {
										madeHand.strength = TP_SPLIT;
									}
									break;

								case OP_TOP_PAIR:
									if (boardPair) {
										madeHand.strength = TP_BOARD_TOP_SIDECARD;
									}
									else {
										madeHand.strength = TP_SPLIT;
									}
									break;

								case OP_LOW_POCKET:			madeHand.strength = TP_LOW_POCKET_BOARD;		break;
								case OP_SECOND_POCKET:		madeHand.strength = TP_SECOND_POCKET_BOARD;		break;
								case OP_OVER_POCKET:		madeHand.strength = TP_OVER_POCKET_BOARD;		break;
								}

								if (i == 4) {
									madeHand.hand[4] = cards[4];
								}
								else {
									madeHand.hand[2] = cards[i - 1];
									madeHand.hand[3] = cards[i - 2];
									madeHand.hand[4] = cards[2];
								}
							} else {
								if (h_hasCardRank(hole, cards[i - 1]) || h_hasCardRank(hole, cards[i - 2])) {
									if (i == 2 || (i == 3 && h_hasCardRank(hole, cards[0]))) {
										madeHand.strength = (pocketPair) ? OP_OVER_POCKET : OP_TOP_PAIR;

										madeHand.hand[0] = cards[0];
										madeHand.hand[1] = cards[1];
										madeHand.hand[2] = cards[2];
										madeHand.hand[3] = cards[3];
										madeHand.hand[4] = cards[4];

										//printf("... top pair!");
									}
									else if (i == 3 || (i == 4 && (h_cardRank(cards[i]) != h_cardRank(cards[i - 1])) && (h_hasCardRank(hole, cards[0]) || h_hasCardRank(hole, cards[1])))) {
										madeHand.strength = (pocketPair) ? OP_SECOND_POCKET : OP_MIDDLE_PAIR;

										madeHand.hand[0] = cards[1];
										madeHand.hand[1] = cards[2];
										madeHand.hand[2] = cards[0];
										madeHand.hand[3] = cards[3];
										madeHand.hand[4] = cards[4];

										//printf("... middle pair!");
									}
									else {
										madeHand.strength = (pocketPair) ? OP_LOW_POCKET : OP_LOW_PAIR;

										madeHand.hand[0] = cards[i - 1];
										madeHand.hand[1] = cards[i - 2];
										madeHand.hand[2] = cards[0];
										madeHand.hand[3] = cards[1];
										madeHand.hand[4] = cards[2];

										//printf("... low pair!");
									}
								}
								else {	// one pair on board
									madeHand.hand[0] = cards[i - 1];
									madeHand.hand[1] = cards[i - 2];
									for (int j = 0; j < i - 2; j++) {
										madeHand.hand[2 + j] = cards[j];
										if ((j + 1) == (i - 2)) {
											for (int k = i; 2 + j + k < 5; k++) {
												madeHand.hand[2 + j + k] = cards[k];
											}
										}
									}

									madeHand.strength = OP_BOARD_PAIR;
									for (int j = 0; j < 3; j++) {
										if (h_hasCard(hole, madeHand.hand[2 + j])) {
											madeHand.strength = OP_BOARD_PAIR_KICKER;
										}
									}
								}
							}
						}
						else if (runLength == 3) {
							//printf("... found three of a kind!");
							madeHand.strength = TK_SET;
						}
						else if (runLength == 4) {
							//printf("... found four of a kind!");
						}

						runLength = 0;
					}
				}

				t_printMadeHand(madeHand);
				printf("\n");
			}

			printf("\n");
		}
	}
}

madeHand_t t_newBlankMadeHand()
{
	madeHand_t hand;

	hand.hand[0] = h_newBlankCard();
	hand.hand[1] = h_newBlankCard();
	hand.hand[2] = h_newBlankCard();
	hand.hand[3] = h_newBlankCard();
	hand.hand[4] = h_newBlankCard();

	hand.strength = NO_STRENGTH;
	return hand;
}

void t_printMadeHand(madeHand_t madeHand) {
	printf("cards: ");
	for (int i = 0; i < 5; i++) {
		h_cardPrintString(madeHand.hand[i]);
	}
	printf(" value: ");
	switch (madeHand.strength) {
	case NO_STRENGTH: printf("nothing"); break;
	case OC_ONE_OVERCARD: printf("one overcard"); break;
	case OC_TWO_OVERCARDS: printf("two overcards"); break;
	case OC_KING_HIGH: printf("king high"); break;
	case OC_ACE_HIGH: printf("ace high"); break;
	case OP_BOARD_PAIR: printf("one pair (board pair no kicker)"); break;
	case OP_BOARD_PAIR_KICKER: printf("one pair (board pair with kicker)"); break;
	case OP_LOW_PAIR: printf("one pair (low pair)"); break;
	case OP_LOW_POCKET: printf("one pair (low pocket pair)"); break;
	case OP_MIDDLE_PAIR: printf("one pair (middle pair)"); break;
	case OP_SECOND_POCKET: printf("one pair (second pocket pair)"); break;
	case OP_TOP_PAIR: printf("top pair"); break;
	case OP_OVER_POCKET: printf("over pocket pair"); break;
	case TP_BOARD: printf("two pair (board pair no kicker)"); break;
	case TP_BOARD_KICKER: printf("two pair (board pair with kicker)"); break;
	case TP_BOARD_LOW_SIDECARD: printf("two pair (board pair low sidecard)"); break;
	case TP_BOARD_MIDDLE_SIDECARD: printf("two pair (board pair middle sidecard)"); break;
	case TP_BOARD_TOP_SIDECARD: printf("two pair (board pair top sidecard)"); break;
	case TP_LOW_POCKET_BOARD: printf("two pair (board pair, low pocket pair)"); break;
	case TP_SECOND_POCKET_BOARD: printf("two pair (board pair, second pocket pair)"); break;
	case TP_OVER_POCKET_BOARD: printf("two pair (over pocket pair, board pair)"); break;
	case TP_SPLIT: printf("two pair (split)"); break;
	case TK_TRIPS_BOARD: printf("trips on board"); break;
	case TK_TRIPS: printf("trips"); break;
	case TK_SET: printf("set"); break;
	}
}