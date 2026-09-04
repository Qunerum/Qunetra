#ifndef STORAGE_H
#define STORAGE_H

#include "../../utility/types.h"

char* initStorage();
void closeStorage();
void writeData(uint64 id, uint8* data, uint64* len);

#endif
