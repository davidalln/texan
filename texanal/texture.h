#pragma once

#include "hand.h"
#include "range.h"

typedef struct {
	card_t cards[5];
	unsigned char length;
} texture_t;

typedef enum {
	// three of a kind
	TK_SET, TK_TRIPS, TK_TRIPS_BOARD,

	// two pair
	TP_SPLIT, TP_OVER_POCKET_BOARD, TP_SECOND_POCKET_BOARD, TP_LOW_POCKET_BOARD, TP_BOARD_TOP_SIDECARD, TP_BOARD_MIDDLE_SIDECARD, TP_BOARD_LOW_SIDECARD, TP_BOARD_KICKER, TP_BOARD,

	// one pair
	OP_OVER_POCKET, OP_TOP_PAIR, OP_SECOND_POCKET, OP_MIDDLE_PAIR, OP_LOW_POCKET, OP_LOW_PAIR, OP_BOARD_PAIR_KICKER, OP_BOARD_PAIR,

	// overcards
	OC_ACE_HIGH, OC_KING_HIGH, OC_TWO_OVERCARDS, OC_ONE_OVERCARD,

	NO_STRENGTH
} strength_t;

#define OP_MIN OP_OVER_POCKET
#define OP_MAX OP_BOARD_PAIR

typedef struct {
	strength_t strength;
	card_t hand[5];
} madeHand_t;

typedef struct tr_node_t tr_node_t;
struct tr_node_t {
	hand_t hole;
	madeHand_t madeHand;

	tr_node_t * next;
	tr_node_t * prev;
};

texture_t t_newBlankTexture();

void t_applyTextureToRange(texture_t texture, range_t range);

madeHand_t t_newBlankMadeHand();
void t_printMadeHand(madeHand_t madeHand);