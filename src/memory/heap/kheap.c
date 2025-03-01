#include "../../../inc/memory/heap/kheap.h"
#include "../../../inc/memory/heap/heap.h"
#include "../../../inc/startup/startup.h"
#include "../../../inc/memory/memory.h"
#include "../../../inc/utils/utils.h"
#include "../../../inc/config.h"
#include "../../../inc/stdio.h"

Heap_t kernel_heap;
HeapTable_t kernel_heap_table;

static uint8_t validate_entry(size_t size, SMAP_entry_t* entry)
{
    uint32_t* addr =  (uint32_t*) entry -> BaseL;
    uint32_t* safe_addr = (uint32_t*) (HEAP_SAFE_ADDRESS);

    return (addr >= safe_addr) && (entry -> BaseH == (ZERO)) &&
           (size >= (OS_HEAP_SIZE_BYTES)) && (entry -> Type == (MEMORY_MAP_ENTRY_FREE));
}

static void choose_mem(size_t entry_size, MEM_entry_t* result, SMAP_entry_t* entry)
{
    uint32_t* addr =  (uint32_t*) entry -> BaseL;
    if (result -> size != (ZERO) && result -> size <= entry_size)
    {
        return;
    }

    void* mem_end = addr + entry_size;
    if (validate_alignement(addr, (OS_HEAP_BLOCK_SIZE)) && 
        validate_alignement(mem_end, (OS_HEAP_BLOCK_SIZE)))
    {
        result -> address = addr;
        result -> size = entry_size;
    }
}

static MEM_entry_t memory_detect()
{
    SMAP_entry_t* entries = (SMAP_entry_t*) (MEMORY_MAP_ENTRIES_ADDR);
    uint16_t* mem_map_len = (uint16_t*) (MEMORY_MAP_COUNT_ADDR);
    MEM_entry_t result = { (ZERO), (ZERO) };
    for (size_t i = (ZERO); i < *mem_map_len; i++)
    {
        SMAP_entry_t entry = entries[i];
        size_t entry_size = (entry.LengthH * (BYTES_TO_MB_MULTIPLIER)) + entry.LengthL;
        if (validate_entry(entry_size, &entry))
        {
            choose_mem(entry_size, &result, &entry);
        }
    }

    return result;
}

void kheap_init()
{
    MEM_entry_t mem_entry = memory_detect();
    mem_entry.address = (void*) 0x01000000u;
    if (mem_entry.size == (ZERO))
    {
        print((uint8_t*) "Failed to create heap\n");
        return;
    }

    uint16_t total_table_entries = mem_entry.size / (OS_HEAP_BLOCK_SIZE);
    kernel_heap_table.entries = (uint8_t*) (OS_HEAP_TABLE_ADDRESS);
    kernel_heap_table.total = total_table_entries;

    void* end = mem_entry.address + mem_entry.size; // pointer to the end of our table
    int16_t res = heap_create(&kernel_heap, mem_entry.address, end, &kernel_heap_table);
    if (res < (ZERO))
    {
        print((uint8_t*) "Failed to create heap\n");
    }

    update_progress((PROGRESS_KHEAP_FINISHED));
}

void* kmalloc(size_t size)
{
    void* ptr = heap_malloc(&kernel_heap, size);
    return ptr;
}

void* kzalloc(size_t size)
{
    void* ptr = kmalloc(size);

    if (!ptr) return (ZERO);

    memset(ptr, ZERO_HEX, size);

    return ptr;
}

void kfree(void* ptr)
{
    heap_free(&kernel_heap, ptr);
}