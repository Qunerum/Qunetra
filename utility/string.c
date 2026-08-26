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
	while (*src != '\0') {
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
}
void strShiftRight(char* str, const uint size, const uint shift) {
	if (!str || size == 0 || shift == 0) return;
	uint len = strLen(str);
	if (len > size - 1) len = size - 1;
	for (int i = len; i >= 0; i--) {
		uint new_pos = i + shift;
		if (new_pos < size - 1) str[new_pos] = str[i];
	}
	for (uint i = 0; i < shift && i < size - 1; i++) str[i] = ' ';
	str[size - 1] = '\0';
}
void strShiftLeft(char* str, const uint size, const uint shift) {
	if (!str || size == 0 || shift == 0) return;
	uint len = strLen(str);
	if (len > size - 1) len = size - 1;
	if (shift >= len) {
		str[0] = '\0';
		return;
	}
	uint idx = 0;
	for (uint i = shift; i < len; i++) str[idx++] = str[i];
	str[idx] = '\0';
}
// strStartWith , strEndWith , strContains , strFind , strUpper, strLower , strTrim

uint stringTest() {
	uint x = 0;
	x += strLen("Hello, World!") == 13;
	x += strIs("Hello", "Hello") && !strIs("Hello", "World");
	char temp[16];
	strCopy(temp, "This is Qunetra");
	x += strIs(temp, "This is Qunetra");
	strShiftRight(temp, 16, 5);
	x += strIs(temp, "     This is Qu");
	strShiftLeft(temp, 16, 7);
	x += strIs(temp, "is is Qu");
	// x += str;
	// x += str;
	return x;
}
