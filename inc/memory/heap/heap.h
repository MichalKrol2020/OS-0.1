#ifndef HEAP_H
#define HEAP_H
#include "../inc/config.h"
#include <stdint.h>
#include <stddef.h>

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN        0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE         0x00

#define HEAP_BLOCK_HAS_NEXT                  0b10000000 // bitmask for the HAS_N (HAS_N is at first bit)
#define HEAP_BLOCK_IS_FIRST                  0b01000000 // bitmask for the IS_FIRST (IS_FIRST is at second bit)

#define HEAP_SAFE_ADDRESS                    0x01000000u

typedef struct 
{
    uint8_t* entries; // pointer to where our entries are going to be
    size_t total;                    // total number of entries
} HeapTable_t;

typedef struct 
{
    HeapTable_t* table;
    void* saddr;            // start address to reference it through our heap implementation
} Heap_t;

int16_t heap_create(Heap_t* heap, void* ptr, void* end, HeapTable_t* table);
void* heap_malloc(Heap_t* heap, size_t size);
void heap_free(Heap_t* heap, void* ptr);

#endif