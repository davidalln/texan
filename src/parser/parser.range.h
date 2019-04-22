#pragma once


#include "range.h"
#include "combo.h"

range_t * rp_parseRangeFile(char * filename);
unsigned rp_parseRangeJSON(char * data, range_t * range);
unsigned rp_parseRangeString(char * string, range_t * range);
