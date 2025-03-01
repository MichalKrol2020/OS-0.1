#ifndef PAGING_H
#define pAGING_H

#include <stdint.h>

#define PAGING_CACHE_DISABLED           0b00010000 // refers to page table entry (5th bit is ache disabled flag)
#define PAGING_WRITE_THROUGH            0b00001000 // 4t bit is Write-Through
#define PAGING_USER_SUPERVISOR          0b00000100 // if this bit is not set then only the supervisor can access this page
#define PAGIN_READ_WRITE                0b00000010 
#define PAGING_IS_PRESENT               0b00000001

#define PAGING_TOTAL_TABLE_ENTRIES      1024
#define PAGING_PAGE_SIZE                4096

typedef struct 
{
    uint32_t* directory_entry;
} Paging_Chunk_t;

Paging_Chunk_t* paging_new_chunk(uint8_t flag);
void paging_switch(uint32_t* directory);

/**
 * @fn - Do not call enable_paging until you have created
 *       a paging chunk and you have switched to that page
 *       directory. Otherwise the system is going to panic.
 * 
 *       Make sure to switch to a page directory before
 *       enabling paging.
*/
void enable_paging();

#endif