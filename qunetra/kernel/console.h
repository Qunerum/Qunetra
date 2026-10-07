#ifndef CONSOLE_H
#define CONSOLE_H

#include "../utility/types.h"

state initConsole();
void setConsoleState(const state _state);
void setCharSize(const uint32 newSize);
void setCharColor(const uint8 newColor);
void putChar(const char c);
void kprintf(const char *format, ...);

#endif
