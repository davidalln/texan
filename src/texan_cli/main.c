#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include "getopt.h"
#include <windows.h>
#define _strcpy strcpy_s
#else
#include <unistd.h>
#define _strcpy strlcpy
#endif

#include "style.h"

#include "parser.range.h"
#include "ll.h"
#include "range.h"

#define OPT_BUFFER_SIZE 1024

typedef enum {
	RANGE, NONE
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

	int opt;

	char fp_in[OPT_BUFFER_SIZE];
	char fp_out[OPT_BUFFER_SIZE];
	status_t status = NONE;

	fp_in[0] = '\0';
	fp_out[0] = '\0';

	while ((opt = getopt(argc, argv, "rf:s:o:")) != -1) {
		switch (opt) {
		case 'r':
			status = RANGE;
			break;
		case 'f':
			switch (status) {
			case RANGE:
				_strcpy(fp_in, OPT_BUFFER_SIZE, optarg);
				printf("%s", optarg);
				break;
			default:
				fprintf(stderr, "texan_cli: attempted to load file without a destination\n");
				return 1;
			}
			break;
		case 'o':
			switch (status) {
			case RANGE:
				_strcpy(fp_out, OPT_BUFFER_SIZE, optarg);
				break;
			default:
				fprintf(stderr, "texan_cli: attempted to load file without a destination\n");
				return 1;
			}
			break;
		}
	}

	range_t * range = rp_parseRangeFile(fp_in);

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

	range->combos = ll_head(range->combos);
	do {
		combo_t combo = c_newNullCombo();
		ll_get(range->combos, &combo);

		char cstr[10];
		c_toString(combo, cstr);
		printf("%s, ", cstr);
		range->combos = ll_next(range->combos);
	} while (!ll_atHead(range->combos));

	return 1;
}