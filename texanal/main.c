#include <stdio.h>

#include <windows.h>
#include "range.h"
#include "texture.h"
#include "parseRange.h"
#include "parseFlop.h"
#include "style.h"
                                                                               
                                                                               
#define BUFFERSIZE 1024                                                                           


int main(int argc, char *argv[]) {
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

	printf(" ________                     ______                       __            ___    \n");
	printf("|        \\                   /      \\                     |  \\          |   \\   \n");
	printf(" \\$$$$$$$$______   __    __ |  $$$$$$\\ _______    ______  | $$       __  \\$$$\\   \n");
	printf("   | $$  /      \\ |  \\  /  \\| $$__| $$|       \\  |      \\ | $$      |  \\   \\$$\\ \n");
	printf("   | $$ |  $$$$$$\\ \\$$\\/  $$| $$    $$| $$$$$$$\\  \\$$$$$$\\| $$       \\$$   | $$ \n");
	printf("   | $$ | $$    $$  >$$  $$ | $$$$$$$$| $$  | $$ /      $$| $$       __    | $$ \n");
	printf("   | $$ | $$$$$$$$ /  $$$$\\ | $$  | $$| $$  | $$|  $$$$$$$| $$      |  \\ _/  $$ \n");
	printf("   | $$  \\$$     \\|  $$ \\$$\\| $$  | $$| $$  | $$ \\$$    $$| $$      | $$|   $$  \n");
	printf("    \\$$   \\$$$$$$$ \\$$   \\$$ \\$$   \\$$ \\$$   \\$$  \\$$$$$$$ \\$$       \\$  \\$$$   \n");
	printf("\n\n");

	printf("Enter range: ");

	char buffer[BUFFERSIZE];
	fgets(buffer, BUFFERSIZE, stdin);
	if (buffer[strlen(buffer) - 1] == '\n') {
		buffer[strlen(buffer) - 1] = '\0';
	}

	range_t range = u_parseRange(buffer);
	r_printRange(range);

	range = r_fillHandList(range);

	printf("Enter flop: ");
	memset(buffer, 0, BUFFERSIZE);
	fgets(buffer, BUFFERSIZE, stdin);
	if (buffer[strlen(buffer) - 1] == '\n') {
		buffer[strlen(buffer) - 1] = '\0';
	}

	flop_t flop = u_parseFlop(buffer);
	f_printString(flop);

	range = r_applyDeadCards(range, flop.card0, flop.card1, flop.card2, h_newBlankCard(), h_newBlankCard());
	r_printCombos(range);
	r_printRange(range);

	texture_t texture = t_newBlankTexture();
	texture.cards[0] = flop.card0;
	texture.cards[1] = flop.card1;
	texture.cards[2] = flop.card2;
	texture.length = 3;

	t_applyTextureToRange(texture, range);

	u_resetStyle();
	//getchar();
	return 0;
}