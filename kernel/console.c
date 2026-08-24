#include "../utility/types.h"

static uint32 cursorX = 0, cursorY = 0;

void putChar(char c) {
	if (c == '\0') return;
	if (c == '\n') {
		cursorX = 0;
		cursorY++;
		return;
	}
}

