#include "../../../inc/memory/paging/paging.h"
#include "../../../inc/memory/heap/kheap.h"
#include "../../../inc/utils/utils.h"

void paging_load_directory(uint32_t* directory);

static uint32_t* current_directory = (ZERO);

void allocate_entry(uint32_t* directory_entry, uint64_t offset, uint8_t flag)
{
    uint32_t* entry = kzalloc(sizeof(uint32_t) * (PAGING_TOTAL_TABLE_ENTRIES));
    for (uint16_t i = (ZERO); i < (PAGING_TOTAL_TABLE_ENTRIES); i++)
    {
        entry[i] = (offset + (i * (PAGING_PAGE_SIZE))) | flag;
    }

    *directory_entry = (uint32_t) entry | flag | (PAGIN_READ_WRITE); // make page writable by default
}

/**
 * @fn - creates a page directory with a bunch of page tables that 
 *       essentially cover the entire four gigabytes of RAM.
 *       
 *       After that we can manipulate these values any way we like to 
 *       point virtual addresses to anywhere in physical memory we like.
*/
Paging_Chunk_t* paging_new_chunk(uint8_t flag)
{
    uint32_t* directory = (uint32_t*) kzalloc(sizeof(uint32_t) * (PAGING_TOTAL_TABLE_ENTRIES));
    uint64_t offset = (ZERO);
    for (uint16_t i = (ZERO); i < (PAGING_TOTAL_TABLE_ENTRIES); i++)
    {
        allocate_entry(&directory[i], offset, flag);
        offset += (PAGING_TOTAL_TABLE_ENTRIES) * (PAGING_PAGE_SIZE);
    }

    Paging_Chunk_t* chunk = (Paging_Chunk_t*) kzalloc(sizeof(Paging_Chunk_t));
    chunk -> directory_entry = directory;

    return chunk;
}

void paging_switch(uint32_t* directory)
{
    paging_load_directory(directory);
    current_directory = directory;
}


