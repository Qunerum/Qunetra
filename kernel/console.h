#ifndef CONSOLE_H
#define CONSOLE_H

#include "../utility/types.h"

void setCharSize(uint32 newSize);
void setCharColor(uint8 newColor);
void putChar(char c);

void kprintf(const char *format, ...);

#endif
