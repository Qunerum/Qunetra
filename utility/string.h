#ifndef STRING_H
#define STRING_H

#include "types.h"

uint strLen(const char* str);
state strIs(const char* strA, const char* strB);
void strCopy(char* dest, const char* src);
void strShiftRight(char* str, const uint size, const uint shift);
void strShiftLeft(char* str, const uint size, const uint shift);
state strStartWith(const char* str, const char* prefix);
state strEndWith(const char* str, const char* suffix);
state strContains(const char* str, const char* sub);
int strFind(const char* str, const char c);
void strUpper(char* str);
void strLower(char* str);
void strTrimStartChar(char* str, const char c);
void strTrimEndChar(char* str, const char c);
void strTrimStart(char* str);
void strTrimEnd(char* str);
void strTrim(char* str);

// X - 5
uint stringTest();

#endif
