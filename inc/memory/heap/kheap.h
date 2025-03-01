#ifndef KHEAP_H
#define KHEAP_H

#include <stdint.h>
#include <stddef.h>

#define MEMORY_MAP_COUNT_ADDR       0x8000u
#define MEMORY_MAP_ENTRIES_ADDR     0x8004u
#define MEMORY_MAP_ENTRY_FREE		1

typedef struct 
{
	uint32_t BaseL; // base address uint64_t
	uint32_t BaseH;
	uint32_t LengthL; // length uint64_t
	uint32_t LengthH;
	uint32_t Type; // entry Type
	uint32_t ACPI; // extended
}__attribute__((packed)) SMAP_entry_t;

typedef struct
{
    void* address;
    size_t size;
} MEM_entry_t;

void kheap_init();
void* kmalloc(size_t size);
void* kzalloc(size_t size);
void kfree(void* ptr);

#endif