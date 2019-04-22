#pragma once


#include "range.h"

range_t * rp_parseRangeFile(char * filename);
unsigned rp_parseRangeJSON(char * data, range_t * range);
