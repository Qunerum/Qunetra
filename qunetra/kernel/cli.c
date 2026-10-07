#include "../utility/types.h"
#include "console.h"

static uint16 inputLength = 0;
static char input[1025];
static state CLIDisabled = false;
void clearCLI() {
	input[0] = '\0';
	while (inputLength > 0) {
		putChar('\b');
		inputLength--;
	}
}
void setCLIState(const state _state) {
	CLIDisabled = !_state;
	if (CLIDisabled && inputLength > 0) clearCLI();
}

void runCmd(const char* cmd, uint cmdLen) {
	putChar('\n');
	if (inputLength > 0) {
		kprintf("%qUnkown command! ['%s']%q\n", 20, cmd, 13);
		inputLength = 0;
		input[0] = '\0';
	}
}
void setCharCLI(const char c) {
	if (CLIDisabled || c == '\0') return;
	if (c == '\n') {
		runCmd(input, sizeof(input));
		return;
	}
	putChar(c);
	if (c == '\b') {
		if (inputLength > 0) inputLength--;
		input[inputLength] = '\0';
	} else {
		input[inputLength] = c;
		inputLength++;
		input[inputLength] = '\0';
	}
}
