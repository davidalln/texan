#include <wchar.h>
#include "style.h"

#define COLOR_BLANK			L"\033[0m"

#define COLOR_FG_BLACK		L"\x1b[30m"
#define COLOR_FG_RED		L"\x1b[31m"
#define COLOR_FG_GREEN		L"\x1b[32m"
#define COLOR_FG_YELLOW		L"\x1b[33m"
#define COLOR_FG_BLUE		L"\x1b[34m"
#define COLOR_FG_MAGENTA	L"\x1b[35m"
#define COLOR_FG_CYAN		L"\x1b[36m"
#define COLOR_FG_WHITE		L"\x1b[37m"
#define COLOR_FG_EXTENDED	L"\x1b[38m"
#define COLOR_FG_DEFAULT	L"\x1b[39m"

#define COLOR_BG_BLACK		L"\x1b[100m"
#define COLOR_BG_RED		L"\x1b[101m"
#define COLOR_BG_GREEN		L"\x1b[102m"
#define COLOR_BG_YELLOW		L"\x1b[103m"
#define COLOR_BG_BLUE		L"\x1b[104m"
#define COLOR_BG_MAGENTA	L"\x1b[105m"
#define COLOR_BG_CYAN		L"\x1b[106m"
#define COLOR_BG_WHITE		L"\x1b[107m"
#define COLOR_BG_EXTENDED	L"\x1b[108m"
#define COLOR_BG_DEFAULT	L"\x1b[109m"

void u_setStyle(color_t fg, color_t bg) {
	switch (fg) {
	case BLACK:		wprintf(L"%ls", COLOR_FG_BLACK);	break;
	case RED:		wprintf(L"%ls", COLOR_FG_RED);		break;
	case GREEN:		wprintf(L"%ls", COLOR_FG_GREEN);	break;
	case YELLOW:	wprintf(L"%ls", COLOR_FG_YELLOW);	break;
	case BLUE:		wprintf(L"%ls", COLOR_FG_BLUE);		break;
	case MAGENTA:	wprintf(L"%ls", COLOR_FG_MAGENTA);	break;
	case CYAN:		wprintf(L"%ls", COLOR_FG_CYAN);		break;
	case WHITE:		wprintf(L"%ls", COLOR_FG_WHITE);	break;
	case EXTENDED:	wprintf(L"%ls", COLOR_FG_EXTENDED); break;
	case DEFAULT:	wprintf(L"%ls", COLOR_FG_DEFAULT);	break;
	}


	switch (bg) {
	case BLACK:		wprintf(L"%ls", COLOR_BG_BLACK);	break;
	case RED:		wprintf(L"%ls", COLOR_BG_RED);		break;
	case GREEN:		wprintf(L"%ls", COLOR_BG_GREEN);	break;
	case YELLOW:	wprintf(L"%ls", COLOR_BG_YELLOW);	break;
	case BLUE:		wprintf(L"%ls", COLOR_BG_BLUE);		break;
	case MAGENTA:	wprintf(L"%ls", COLOR_BG_MAGENTA);	break;
	case CYAN:		wprintf(L"%ls", COLOR_BG_CYAN);		break;
	case WHITE:		wprintf(L"%ls", COLOR_BG_WHITE);	break;
	case EXTENDED:	wprintf(L"%ls", COLOR_BG_EXTENDED); break;
	case DEFAULT:	wprintf(L"%ls", COLOR_BG_DEFAULT);	break;
	}
}

void u_resetStyle() {
	wprintf(L"%ls", COLOR_BLANK);
}
