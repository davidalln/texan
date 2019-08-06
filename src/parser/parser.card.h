#pragma once

#include "card.h"

#define MAX_CARD_STRING_LENGTH 5

unsigned dp_parseCardString(const char * string, size_t BUFFER_SIZE, card_t * cards);