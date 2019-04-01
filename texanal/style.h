#pragma once

typedef enum {
	BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, EXTENDED, DEFAULT
} color_t;

void u_setStyle(color_t fg, color_t bg);
void u_resetStyle();