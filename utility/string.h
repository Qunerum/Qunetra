#ifndef STRING_H
#define STRING_H

#include "types.h"

uint strLen(const char* str);
state strIs(const char* strA, const char* strB);
void strCopy(char* dest, const char* src);
void strShiftRight(char* str, const uint size, const uint shift);
void strShiftLeft(char* str, const uint size, const uint shift);

uint stringTest();

#endif
