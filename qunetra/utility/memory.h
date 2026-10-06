#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

void *kmalloc(vsize size);
void kfree(void *ptr);
void *krealloc(void *ptr, vsize new_size);

uint memoryTest();

#endif
