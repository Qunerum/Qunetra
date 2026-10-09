#include "../utility/types.h"
#include "console.h"

#define CMD_MAX_LEN 128
#define CMDL_STEP 256

static uint16 inputLength = 0;
static char input[CMD_MAX_LEN + 1];
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

void addCmd(const char* cmd) {

}

void runCmd(const char* cmd, uint cmdLen) {
	if (inputLength == 0) return;
	putChar('\n');


	kprintf("%qUnkown command! ['%s']%q\n", 20, cmd, 13);
	inputLength = 0;
	input[0] = '\0';
}
void setCharCLI(const char c) {
	if (CLIDisabled || c == '\0') return;
	if (c == '\n') {
		runCmd(input, inputLength);
		return;
	}
	if (c == '\b') {
		putChar('\b');
		if (inputLength > 0) inputLength--;
		input[inputLength] = '\0';
		return;
	}
	if (inputLength >= CMD_MAX_LEN) return;
	putChar(c);
	input[inputLength] = c;
	inputLength++;
	input[inputLength] = '\0';
}
