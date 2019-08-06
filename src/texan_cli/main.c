#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include "getopt.h"
#include <windows.h>
#define _strcpy strcpy_s
#define _strlen(a,b) strnlen_s(a,b)
#else
#include <unistd.h>
#define _strcpy strlcpy
#endif

#include "style.h"

#include "parser.range.h"
#include "ll.h"
#include "range.h"
#include "equity.h"
#include "deck.h"

#define OPT_BUFFER_SIZE 1024
#define COMMUNITY_BUFFER_SIZE 32

#define MAX_RANGES 2

typedef enum {
	UNLOADED_RANGE, LOADED_RANGE, COMMUNITY, NONE
} status_t;

int main(int argc, char * argv[]) {
#ifdef _WIN32 
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		return GetLastError();
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		return GetLastError();
	}
#endif

	srand(time(NULL));

	int opt;

	char range_fp_in[OPT_BUFFER_SIZE] = { '\0' };
	char range_fp_out[OPT_BUFFER_SIZE] = { '\0' };
	char range_string[OPT_BUFFER_SIZE] = { '\0' };
	char community_string[COMMUNITY_BUFFER_SIZE] = { '\0' };
	char range_name[MAX_RANGE_NAME_SIZE] = { '\0' };
	status_t status = NONE;

	community_string[0] = '\0';

	range_t * ranges[MAX_RANGES];
	unsigned atRange = 0;
	unsigned nRanges = 0;

	while ((opt = getopt(argc, argv, "cs:rf:s:o:n:")) != -1) {
		switch (opt) {
		case 'r':
			status = UNLOADED_RANGE;
			range_name[0] = '\0';
			break;
		case 'c':
			status = COMMUNITY;
			break;
		case 'f':
			switch (status) {
			case UNLOADED_RANGE:
				_strcpy(range_fp_in, OPT_BUFFER_SIZE, optarg);
				if (_strlen(range_fp_in, OPT_BUFFER_SIZE) > 0) {
					range_t * range = rp_parseRangeFile(range_fp_in);
					if (!range) {
						fprintf(stderr, "texan_cli: could not parse range file %s\n", range_fp_in);
						return 1;
					}

					atRange = nRanges++;
					if (nRanges > MAX_RANGES) {
						fprintf(stderr, "texan_cli: attempted to load too many ranges\n");
						return 1;
					}
					ranges[atRange] = range;

					if (_strlen(range_name, OPT_BUFFER_SIZE) > 0) {
						_strcpy(ranges[atRange]->name, MAX_RANGE_NAME_SIZE, range_name);
					}

					status = LOADED_RANGE;
				}
				break;
			default:
				fprintf(stderr, "texan_cli: attempted to load file without a source\n");
				return 1;
			} break;
		case 'o':
			switch (status) {
			case UNLOADED_RANGE:
				_strcpy(range_fp_out, OPT_BUFFER_SIZE, optarg);
				break;
			default:
				fprintf(stderr, "texan_cli: attempted to output file without a source\n");
				return 1;
			} break;
		case 's':
			switch (status) {
			case UNLOADED_RANGE:
				_strcpy(range_string, OPT_BUFFER_SIZE, optarg);
				if (_strlen(range_string, OPT_BUFFER_SIZE) > 0) {
					range_t * range = r_newRange();
					rp_parseRangeString(range_string, range);

					atRange = nRanges++;
					if (nRanges > MAX_RANGES) {
						fprintf(stderr, "texan_cli: attempted to load too many ranges\n");
						return 1;
					}
					ranges[atRange] = range;

					if (_strlen(range_name, OPT_BUFFER_SIZE) > 0) {
						_strcpy(ranges[atRange]->name, MAX_RANGE_NAME_SIZE, range_name);
					}

					status = LOADED_RANGE;
				}
				break;
			case COMMUNITY:
				_strcpy(community_string, COMMUNITY_BUFFER_SIZE, optarg);
				break;
			default:
				fprintf(stderr, "texan_cli: attempted to read string without a source\n");
				return 1;
			} break;
		case 'n':
			switch (status) {
			case UNLOADED_RANGE:
				_strcpy(range_name, MAX_RANGE_NAME_SIZE, optarg);
				break;
			case LOADED_RANGE:
				_strcpy(range_name, MAX_RANGE_NAME_SIZE, optarg);
				if (_strlen(range_name, OPT_BUFFER_SIZE) > 0) {
					_strcpy(ranges[atRange]->name, MAX_RANGE_NAME_SIZE, range_name);
				}
				break;
			default:
				fprintf(stderr, "texan_cli: attempted to add a name without a source %d\n", status);
				return 1;
			} break;
		}

	}

	for (int r = 0; r < nRanges; r++) {
		range_t * range = ranges[r];

		printf("+");
		for (int i = 0; i < _strlen(range->name, MAX_RANGE_NAME_SIZE); i++) {
			printf("-");
		}
		printf("+\n|%s|\n", range->name);

		for (int i = 0; i < 13; i++) {
			printf("+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
			for (int j = 0; j < 13; j++) {
				combo_t combo = c_newCombo(i, j);

				printf("|");

				if (r_hasCombo(range, combo)) {
					u_setStyle(BLACK, GREEN);
				}

				char cstr[10];
				c_toString(combo, cstr);
				printf("%s", cstr);

				if (strlen(cstr) == 2) {
					printf(" ");
				}
				printf("");

				u_resetStyle();
			}
			printf("|\n");
		}
		printf("+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
	}

	unsigned nCommunity = 0;
	card_t community[5] = { d_newNullCard(), d_newNullCard(), d_newNullCard(), d_newNullCard(), d_newNullCard() };
	if (_strlen(community_string, COMMUNITY_BUFFER_SIZE) > 0) {
		nCommunity = dp_parseCardString(community_string, COMMUNITY_BUFFER_SIZE, community);

		char str0[10];
		char str1[10];
		char str2[10];
		char str3[10];
		char str4[10];
		d_toString(community[0], str0);
		d_toString(community[1], str1);
		d_toString(community[2], str2);
		d_toString(community[3], str3);
		d_toString(community[4], str4);

		printf("with community cards %s, %s, %s, %s, %s (%d)\n", str0, str1, str2, str3, str4, nCommunity);
	}

	for (int i = 0; i < nRanges; i++) {
		hand_t removedHands[128];
		unsigned nRemovedHands = 0;
		r_deleteCards_hands(ranges[i], community, nCommunity, removedHands, &nRemovedHands);
		printf("removed %d hands\n", nRemovedHands);
	}

	printf("Performing range analysis (HERO: %s, VILLAIN: %s) on board Kd7s2c\n", ranges[0]->name, ranges[1]->name);
	hand_t * hero_hands = r_generateHandArray(ranges[0]);
	deck_t * deck = k_newDeck();

	k_removeCards(deck, community, nCommunity);
	for (int i = 0; i < ranges[0]->nHands; i++) {
		q_calculateHandVsRangeEquity(hero_hands[i], ranges[1], deck, community, nCommunity);
	}

	k_deleteDeck(deck);
	getchar();
	return 0;
}