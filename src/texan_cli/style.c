#include "style.h"

#ifdef _WIN32
	#include <wchar.h>

	#define _L L
	#define SET_COLOR(color) wprintf(L"%ls", color)
#else
	#include <stdio.h>
	#define _L
	#define SET_COLOR(color) printf("%s", color)
#endif

#define COLOR_BLANK		_L"\033[0m"

#define COLOR_FG_BLACK		_L"\x1b[30m"
#define COLOR_FG_RED		_L"\x1b[31m"
#define COLOR_FG_GREEN		_L"\x1b[32m"
#define COLOR_FG_YELLOW		_L"\x1b[33m"
#define COLOR_FG_BLUE		_L"\x1b[34m"
#define COLOR_FG_MAGENTA	_L"\x1b[35m"
#define COLOR_FG_CYAN		_L"\x1b[36m"
#define COLOR_FG_WHITE		_L"\x1b[37m"
#define COLOR_FG_EXTENDED	_L"\x1b[38m"
#define COLOR_FG_DEFAULT	_L"\x1b[39m"

#define COLOR_BG_BLACK		_L"\x1b[100m"
#define COLOR_BG_RED		_L"\x1b[101m"
#define COLOR_BG_GREEN		_L"\x1b[102m"
#define COLOR_BG_YELLOW		_L"\x1b[103m"
#define COLOR_BG_BLUE		_L"\x1b[104m"
#define COLOR_BG_MAGENTA	_L"\x1b[105m"
#define COLOR_BG_CYAN		_L"\x1b[106m"
#define COLOR_BG_WHITE		_L"\x1b[107m"
#define COLOR_BG_EXTENDED	_L"\x1b[108m"
#define COLOR_BG_DEFAULT	_L"\x1b[109m"

#ifdef _WIN32
	#define SET_COLOR(color) wprintf(L"%ls", color)
#else
	#define SET_COLOR(color) printf("%s", color)
#endif

void u_setStyle(color_t fg, color_t bg) {
	switch (fg) {
	case BLACK:		SET_COLOR(COLOR_FG_BLACK); break;
	case RED:		SET_COLOR(COLOR_FG_RED);		break;
	case GREEN:		SET_COLOR(COLOR_FG_GREEN);	break;
	case YELLOW:	 SET_COLOR(COLOR_FG_YELLOW);	break;
	case BLUE:		 SET_COLOR(COLOR_FG_BLUE);		break;
	case MAGENTA:	 SET_COLOR(COLOR_FG_MAGENTA);	break;
	case CYAN:		 SET_COLOR(COLOR_FG_CYAN);		break;
	case WHITE:		 SET_COLOR(COLOR_FG_WHITE);	break;
	case EXTENDED:	 SET_COLOR(COLOR_FG_EXTENDED); break;
	case DEFAULT:	 SET_COLOR(COLOR_FG_DEFAULT);	break;
	}


	switch (bg) {
	case BLACK:		 SET_COLOR(COLOR_BG_BLACK);	break;
	case RED:		 SET_COLOR(COLOR_BG_RED);		break;
	case GREEN:		 SET_COLOR(COLOR_BG_GREEN);	break;
	case YELLOW:	 SET_COLOR(COLOR_BG_YELLOW);	break;
	case BLUE:		 SET_COLOR(COLOR_BG_BLUE);		break;
	case MAGENTA:	 SET_COLOR(COLOR_BG_MAGENTA);	break;
	case CYAN:		 SET_COLOR(COLOR_BG_CYAN);		break;
	case WHITE:		 SET_COLOR(COLOR_BG_WHITE);	break;
	case EXTENDED:	 SET_COLOR(COLOR_BG_EXTENDED); break;
	case DEFAULT:	 SET_COLOR(COLOR_BG_DEFAULT);	break;
	}
}

void u_resetStyle() {
	SET_COLOR(COLOR_BLANK);
}
