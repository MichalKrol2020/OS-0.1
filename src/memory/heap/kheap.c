#include "../../../inc/memory/heap/kheap.h"
#include "../../../inc/memory/heap/heap.h"
#include "../../../inc/config.h"
#include "../../../inc/stdio.h"

Heap_t kernel_heap;
HeapTable_t kernel_heap_table;

void kheap_init()
{
    uint16_t total_table_entries = OS_HEAP_SIZE_BYTES / OS_HEAP_BLOCK_SIZE;
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*) (OS_HEAP_TABLE_ADDRESS);
    kernel_heap_table.total = total_table_entries;

    void* end = (void*) (OS_HEAP_ADDRESS + OS_HEAP_SIZE_BYTES); // pointer to the end of our table
    int res = heap_create(&kernel_heap, (void*) (OS_HEAP_ADDRESS), end, &kernel_heap_table);
    if (res < 0)
    {
        print((uint8_t*) "Failed to create heap\n");
    }
}