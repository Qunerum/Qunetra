#ifndef STORAGE_H
#define STORAGE_H

#include "../../utility/types.h"

char* initStorage();
void closeStorage();
void writeFile(const char* name, const uint64 dirID, const uint8* data, const uint64 len);

#endif
