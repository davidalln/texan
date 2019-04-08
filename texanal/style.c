#include "style.h"

#ifdef _WIN32
	#include <wchar.h>
#else
	#include <stdio.h>
#endif

#define COLOR_BLANK		"\033[0m"

#define COLOR_FG_BLACK		"\x1b[30m"
#define COLOR_FG_RED		"\x1b[31m"
#define COLOR_FG_GREEN		"\x1b[32m"
#define COLOR_FG_YELLOW		"\x1b[33m"
#define COLOR_FG_BLUE		"\x1b[34m"
#define COLOR_FG_MAGENTA	"\x1b[35m"
#define COLOR_FG_CYAN		"\x1b[36m"
#define COLOR_FG_WHITE		"\x1b[37m"
#define COLOR_FG_EXTENDED	"\x1b[38m"
#define COLOR_FG_DEFAULT	"\x1b[39m"

#define COLOR_BG_BLACK		"\x1b[100m"
#define COLOR_BG_RED		"\x1b[101m"
#define COLOR_BG_GREEN		"\x1b[102m"
#define COLOR_BG_YELLOW		"\x1b[103m"
#define COLOR_BG_BLUE		"\x1b[104m"
#define COLOR_BG_MAGENTA	"\x1b[105m"
#define COLOR_BG_CYAN		"\x1b[106m"
#define COLOR_BG_WHITE		"\x1b[107m"
#define COLOR_BG_EXTENDED	"\x1b[108m"
#define COLOR_BG_DEFAULT	"\x1b[109m"

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
