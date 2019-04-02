#pragma once

#include "hand.h"

typedef struct {
	card_t card0;
	card_t card1;
	card_t card2;
	card_t card3;
	card_t card4;
} texture_t;

texture_t t_newTexture();