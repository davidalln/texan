#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.range.h"

#include "json.h"
#include "json_util.h"

range_t * rp_parseRangeFile(char * filename) {
	char * data = texan_load_file(filename);
	if (!data) {
		fprintf(stderr, "rp_parseRangeFile: could not load file %s", filename);
		return NULL;
	}

	range_t * range = r_newRange();
	unsigned result = rp_parseRangeJSON(data, range);
	if (!result) {
		fprintf(stderr, "rp_parseRangeFile: error parsing range JSON\n");
		r_deleteRange(range);
		return NULL;
	}

	return range;
}

json_object * rp_encodeRangeJSON(range_t *range);
signed rp_saveRangeToFile(range_t * range, char * filename) {
	size_t length;
	char * string = json_object_to_json_string_length(rp_encodeRangeJSON(range), 0, &length);
	const char * copy = _strdup(string);
	texan_write_file(filename, string, length);

	return 0;
}

json_object * rp_encodeRangeJSON(range_t *range)
{
	json_object *head_obj;
	head_obj = json_object_new_object();

	if (!r_isNull(*range)) {
		json_object *name_str = json_object_new_string(range->name);
		json_object_object_add(head_obj, "name", name_str);

		json_object *range_arr;
		range_arr = json_object_new_array();

		range->combos = ll_head(range->combos);
		do {
			json_object *combo_obj;
			combo_obj = json_object_new_object();

			combo_t combo = c_newNullCombo();
			ll_get(range->combos, &combo);

			char combo_str[4];
			c_toString(combo, combo_str);
			json_object *combo_name_str = json_object_new_string(combo_str);
			json_object_object_add(combo_obj, "combo", combo_name_str);

			unsigned hand_arr[16] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0
			};

			combo.hands = ll_head(combo.hands);
			printf("%d:", combo.hands.length);
			do {
				hand_t hand = h_newNullHand();
				ll_get(combo.hands, &hand);

				suit_t x = hand.cards[0].suit;
				suit_t y = hand.cards[1].suit;

				printf("%d is 1:", x * 4 + y);
				hand_arr[x * 4 + y] = 1;

				combo.hands = ll_next(combo.hands);
			} while (!ll_atHead(combo.hands));
			printf("\n");

			json_object *combo_hand_arr = json_object_new_array();
			for (int i = 0; i < 4; i++) {
				json_object *combo_hand_arr_row = json_object_new_array();
				for (int j = 0; j < 4; j++) {
					json_object_array_add(combo_hand_arr_row, json_object_new_boolean(hand_arr[i * 4 + j]));
				}
				json_object_array_add(combo_hand_arr, combo_hand_arr_row);
			}

			printf("%s\n", json_object_to_json_string(combo_hand_arr));

			json_object_object_add(combo_obj, "hands", combo_hand_arr);
			json_object_array_add(range_arr, combo_obj);
			range->combos = ll_next(range->combos);
		} while (!ll_atHead(range->combos));

		json_object_object_add(head_obj, "range", range_arr);
	}

	return head_obj;
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
			case 's': combo_type = SUITED; break;
			case 'o': combo_type = OFFSUIT; break;
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
	}

	return range->combos.length;
}

unsigned rp_parseRangeString(char * string, range_t * range)
{
	rank_t highRanks[2] = { NULL_RANK, NULL_RANK };
	rank_t lowRanks[2] = { NULL_RANK, NULL_RANK };
	unsigned atEnd = 0;
	char * c = string;
	combo_type_t atType = NULL_COMBO;

	while (!atEnd) {
		if (*c == '\0')
			atEnd = 1;

		rank_t atRank = NULL_RANK;
		switch (*c) {
		case 'A': atRank = ACE; break;
		case 'K': atRank = KING; break;
		case 'Q': atRank = QUEEN; break;
		case 'J': atRank = JACK; break;
		case 'T': atRank = TEN; break;
		case '9': atRank = NINE; break;
		case '8': atRank = EIGHT; break;
		case '7': atRank = SEVEN; break;
		case '6': atRank = SIX; break;
		case '5': atRank = FIVE; break;
		case '4': atRank = FOUR; break;
		case '3': atRank = THREE; break;
		case '2': atRank = TWO; break;

		case 's':
			atType = SUITED; break;

		case 'o':
			atType = OFFSUIT; break;

		case '+':
			if (highRanks[0] == NULL_RANK || highRanks[1] == NULL_RANK) {
				fprintf(stderr, "rp_parseRangeString: Invalid range string (tried to use %c without enough data)\n", *c);
				return 0;
			}
			else {
				switch (atType) {
				case PAIR: lowRanks[0] = ACE; lowRanks[1] = ACE; break;
				case SUITED: case OFFSUIT: lowRanks[0] = highRanks[0]; lowRanks[1] = highRanks[0] + 1; break;
				default:
					fprintf(stderr, "rp_parseRangeString: Invalid range string (not enough data for action %c)\n", *c);
					return 0;
				}
			} break;

		case '-':
			if (highRanks[0] == NULL_RANK || highRanks[1] == NULL_RANK) {
				fprintf(stderr, "rp_parseRangeString: Invalid range string (not enough data for action %c)\n", *c);
				return 0;
			}
			else {
				lowRanks[0] = highRanks[0];
				lowRanks[1] = highRanks[1];
				highRanks[0] = NULL_RANK;
				highRanks[1] = NULL_RANK;
			} break;

		case ' ': case ',': case '\0':
			if (highRanks[0] != NULL_RANK && highRanks[1] != NULL_RANK) {
				if (lowRanks[0] == NULL_RANK || lowRanks[1] == NULL_RANK) {
					lowRanks[0] = highRanks[0];
					lowRanks[1] = highRanks[1];
				}

				for (int i = 0; i <= highRanks[1] - lowRanks[1]; i++) {
					switch (atType) {
					case PAIR:
						r_addCombo(range, c_newCombo(lowRanks[0] + i, lowRanks[1] + i)); break;
					case SUITED:
						r_addCombo(range, c_newCombo(lowRanks[0], lowRanks[1] + i)); break;
					case OFFSUIT:
						r_addCombo(range, c_newCombo(lowRanks[1] + i, lowRanks[0])); break;
					case NULL_COMBO:
						fprintf(stderr, "rp_parseRangeString: Invalid range string (not enough data to add combo)\n");
						return 0;
					}
				}

				highRanks[0] = NULL_RANK;
				highRanks[1] = NULL_RANK;
				lowRanks[0] = NULL_RANK;
				lowRanks[1] = NULL_RANK;
				atType = NULL_COMBO;
			}
			else if (highRanks[0] != NULL_RANK || highRanks[1] != NULL_RANK) {
				fprintf(stderr, "rp_parseRangeString: Invalid range string (not enough data to add combo)\n");
				return 0;
			} break;

		default:
			fprintf(stderr, "rp_parseRangeString: Invalid range string (cannot process character %c)", *c);
			return 0;
		}

		if (atRank != NULL_RANK) {
			if (highRanks[0] == NULL_RANK) highRanks[0] = atRank;
			else if (highRanks[1] == NULL_RANK) {
				highRanks[1] = atRank;
				if (highRanks[0] == highRanks[1]) atType = PAIR;
			}
			else if (lowRanks[0] == NULL_RANK) lowRanks[0] = atRank;
			else if (lowRanks[1] == NULL_RANK) {
				lowRanks[1] = atRank;
				if (highRanks[0] < lowRanks[0] || highRanks[1] < lowRanks[1]) {
					rank_t tmp = highRanks[0];
					highRanks[0] = lowRanks[0];
					lowRanks[0] = tmp;

					tmp = highRanks[1];
					highRanks[1] = lowRanks[1];
					lowRanks[1] = tmp;
				}
			}

			atRank = NULL_RANK;
		}

		c++;
	}

	return range->combos.length;
}
