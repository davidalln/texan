#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include "getopt.h"
#define _strcpy strcpy_s
#else
#include <unistd.h>
#define _strcpy strlcpy
#endif

#define OPT_BUFFER_SIZE 1024

typedef enum {
	RANGE, NONE
} status_t;

int main(int argc, char * argv[]) {
	int opt;

	char fp_in[OPT_BUFFER_SIZE];
	char fp_out[OPT_BUFFER_SIZE];
	status_t status = NONE;

	fp_in[0] = '\0';
	fp_out[0] = '\0';

	while ((opt = getopt(argc, argv, ":rf:s:o:")) != -1) {
		switch (opt) {
		case 'r':
			status = RANGE;
			break;
		case 'f':
			switch (status) {
			case RANGE:
				_strcpy(fp_in, OPT_BUFFER_SIZE, optarg);
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
}