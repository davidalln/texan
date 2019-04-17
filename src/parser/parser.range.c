#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.range.h"

#include "json.h"

range_t * rp_parseRangeFile(char * filename) {
	char * data = texan_load_file(filename);
	if (!data) {
		fprintf(stderr, "rp_parseRangeFile: could not load file %s", filename);
		return NULL;
	}

	range_t * range = r_newRange();
	unsigned result = rp_parseRangeJSON(data, &range);
	if (!result) {
		fprintf(stderr, "rp_parseRangeFile: error parsing range JSON\n");
		r_deleteRange(range);
		return NULL;
	}

	return range;
}

unsigned rp_parseRangeJSON(char * data, range_t * range)
{
	json_object *head_obj;
	head_obj = json_tokener_parse(data);

	if (!head_obj) {
		fprintf(stderr, "rp_parseRange: Invalid range file (no head json object)\n");
		return 0;
	}

	json_type obj_type = json_object_get_type(head_obj);
	if (obj_type != json_type_object) {
		fprintf(stderr, "rp_parseRange: Invalid range file (head is not type json object)\n");
		return 0;
	}

	json_object *name = json_object_object_get(head_obj, "name");
	if (name) {
		obj_type = json_object_get_type(name);
		if (obj_type != json_type_string) {
			fprintf(stderr, "rp_parseRange: Invalid range file (name is not type json string)\n");
			return 0;
		}

		strcpy_s(range->name, MAX_RANGE_NAME_SIZE, json_object_get_string(name));
	}

	json_object *json_range = json_object_object_get(head_obj, "range");
	if (!json_range) {
		fprintf(stderr, "rp_parseRange: Invalid range file (no range json object)\n");
		return 0;
	}

	obj_type = json_object_get_type(json_range);
	if (obj_type != json_type_array) {
		fprintf(stderr, "rp_parseRange: Invalid range file (range is not type json array)\n");
		return 0;
	}

	int nCombos = json_object_array_length(json_range);
	for (int i = 0; i < nCombos; i++) {
		json_object *combo = json_object_array_get_idx(json_range, i);
		obj_type = json_object_get_type(combo);

		if (obj_type != json_type_object) {
			fprintf(stderr, "rp_parseRange: Invalid range file (combo is not type json object)\n");
			return 0;
		}

		json_object *combo_string = json_object_object_get(combo, "combo");
		if (!combo_string) {
			fprintf(stderr, "rp_parseRange: Invalid range file (combo does not have a combo string)\n");
			return 0;
		}

		obj_type = json_object_get_type(combo_string);
		if (obj_type != json_type_string) {
			fprintf(stderr, "rp_parseRange: Invalid range file (combo string is not type json string)\n");
			return 0;
		}

		char * string = json_object_get_string(combo_string);
		if (strlen(string) < 2 || strlen(string) > 3) {
			fprintf(stderr, "rp_parseRange: Invalid range file (combo string %s has invalid length %d)\n", string, strlen(string));
			return 0;
		}

		json_object *combo_hands = json_object_object_get(combo, "hands");
		if (!combo_string) {
			fprintf(stderr, "rp_parseRange: Invalid range file (combo %s does not have a hands array)\n", string);
			return 0;
		}

		obj_type = json_object_get_type(combo_hands);
		if (obj_type != json_type_array) {
			fprintf(stderr, "rp_parseRange: Invalid range file (combo %s hands is not type json array)\n", string);
			return 0;
		}

		int hands_array_length = json_object_array_length(combo_hands);
		if (hands_array_length != 4) {
			fprintf(stderr, "rp_parseRange: Invalid range file (combo %s has hands array length %d)\n", string, hands_array_length);
			return;
		}

		char _rank0 = string[0];
		char _rank1 = string[1];
		
		rank_t rank0 = NULL_RANK;
		rank_t rank1 = NULL_RANK;

		switch (_rank0) {
		case 'A': rank0 = ACE; break;
		case 'K': rank0 = KING; break;
		case 'Q': rank0 = QUEEN; break;
		case 'J': rank0 = JACK; break;
		case 'T': rank0 = TEN; break;
		case '9': rank0 = NINE; break;
		case '8': rank0 = EIGHT; break;
		case '7': rank0 = SEVEN; break;
		case '6': rank0 = SIX; break;
		case '5': rank0 = FIVE; break;
		case '4': rank0 = FOUR; break;
		case '3': rank0 = THREE; break;
		case '2': rank0 = TWO; break;
		}

		switch (_rank1) {
		case 'A': rank1 = ACE; break;
		case 'K': rank1 = KING; break;
		case 'Q': rank1 = QUEEN; break;
		case 'J': rank1 = JACK; break;
		case 'T': rank1 = TEN; break;
		case '9': rank1 = NINE; break;
		case '8': rank1 = EIGHT; break;
		case '7': rank1 = SEVEN; break;
		case '6': rank1 = SIX; break;
		case '5': rank1 = FIVE; break;
		case '4': rank1 = FOUR; break;
		case '3': rank1 = THREE; break;
		case '2': rank1 = TWO; break;
		}

		if (rank0 == NULL_RANK) {
			fprintf(stderr, "rp_parseRange: Invalid range file (rank0 %c is invalid in string %s)\n", _rank0, string);
			return 0;
		}

		if (rank1 == NULL_RANK) {
			fprintf(stderr, "rp_parseRange: Invalid range file (rank1 %c is invalid in string %s)\n", _rank1, string);
			return 0;
		}

		combo_type_t combo_type = NULL_COMBO;
		if (strlen(string) == 2) {
			if (rank0 != rank1) {
				fprintf(stderr, "rp_parseRange: Invalid range file (no combo type for combo %s)\n", string);
				return 0;
			}

			combo_type = PAIR;
		}
		else {
			char type = string[2];
			switch (type) {
			case 's': combo_type = SUITED;
			case 'o': combo_type = OFFSUIT;
			}

			if (combo_type == NULL_COMBO) {
				fprintf(stderr, "rp_parseRange: Invalid range file (invalid combo type %c in string %s)\n", type, string);
				return 0;
			}
		}

		combo_t newCombo = c_newBlankCombo(rank0, rank1, combo_type);
		if (newCombo.type == NULL_COMBO) {
			fprintf(stderr, "rp_parseRange: Invalid range file (invalid combo %s)\n", string);
			return 0;
		}

		for (int x = 0; x < 4; x++) {
			json_object *hands_array_array = json_object_array_get_idx(combo_hands, x);
			obj_type = json_object_get_type(hands_array_array);
			if (obj_type != json_type_array) {
				fprintf(stderr, "rp_parseRange: Invalid range file (combo %s hands array array %d not type json array)\n", string, x);
				return 0;
			}

			int hands_array_array_length = json_object_array_length(hands_array_array);
			if (hands_array_array_length != 4) {
				fprintf(stderr, "rp_parseRange: Invalid range file (combo %s hands array array %d has invalid length %d)\n", string, x, hands_array_array_length);
				return 0;
			}

			for (int y = 0; y < 4; y++) {
				json_object *hand_active = json_object_array_get_idx(hands_array_array, y);
				obj_type = json_object_get_type(hand_active);
				if (obj_type != json_type_boolean) {
					fprintf(stderr, "rp_parseRange: Invalid range file (combo %s hand (%d, %d) is not type json bool)\n", string, y, x);
					return 0;
				}
				
				if (json_object_get_boolean(hand_active)) {
					c_addHand(&newCombo, h_newHand(d_newCard(rank0, y), d_newCard(rank1, x)));
				}
			}
		}

		if (newCombo.hands.length == 0) {
			fprintf(stderr, "rp_parseRange: Inavlid range file (combo %s has no active hands)\n", string);
			return 0;
		}

		if (r_addCombo(range, newCombo) == 0) {
			fprintf(stderr, "rp_parseRange: Failed to add combo %s to the range", string);
			return 0;
		}

		char cstr[10];
		c_toString(newCombo, cstr);
		printf("added combo %s with %d hands to the range\n", cstr, newCombo.hands.length);
	}

	return range->combos.length;
}
