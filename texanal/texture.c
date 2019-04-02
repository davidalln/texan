#include "texture.h"

texture_t t_newTexture()
{
	texture_t texture;
	texture.card0 = h_newBlankCard();
	texture.card1 = h_newBlankCard();
	texture.card2 = h_newBlankCard();
	texture.card3 = h_newBlankCard();
	texture.card4 = h_newBlankCard();
	return texture;
}
