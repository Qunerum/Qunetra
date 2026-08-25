#include "types.h"

uint strLen(const char* str) {
	if (!str) return 0;
	uint i = 0;
	while (str[i] != '\0') i++;
	return i;
}
state strIs(const char* strA, const char* strB) {
	if (!strA || !strB) return strA == strB;
	while (*strA != '\0' && (*strA == *strB)) {
		strA++;
		strB++;
	}
	return *strA == *strB;
}
void strCopy(char* dest, const char* src) {
	if (!dest || !src) return;
	while (*dest != '\0' && *src != '\0') {
		*dest = *src;
		dest++;
		src++;
	}
}
void strShiftRight(char* str, const uint size, const uint shift) {
	if (!str || size == 0 || shift == 0) return;
	// to make
}
// strShiftLeft , strShift , strStartWith , strEndWith , strContains , strFind , strUpper, strLower , strTrim
