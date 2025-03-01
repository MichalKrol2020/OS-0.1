#ifndef MEMOTY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

void* memset(void* ptr, uint16_t c, size_t size);
uint8_t validate_alignement(void* ptr, size_t align_bytes);

extern void memcpy(void *dest, const void *src, size_t n);

#endif