#include "../../../inc/memory/heap/heap.h"
#include "../../../inc/memory/memory.h"
#include "../../../inc/status.h"

/**
 * @fn     - And we're just going to validate that the pointer 
 *           they provided and the end address is correct for the table they provided.
 *           And here we check if the person using our heap implementation has miscalculated
 *           it or not, which ispretty important because we can't have a 100 megabyte data 
 *           heap with only ten megabytes of blocks in the heap table. It's going to cause problems.
 *           We're going to overflow and we're going to write into some data we shouldn't write.
 * 
 * @return - true if the ptr, end and address is correct, false otherwise
*/
static uint16_t heap_validate_table(void* ptr, void* end, HeapTable_t* table)
{
    int res = ZERO;
    size_t table_size = (size_t)(end - ptr);
    size_t total_pages = table_size / OS_HEAP_BLOCK_SIZE;
    if (table -> total != total_blocks)
    {
        res = -EINVARG;
        goto out;
    }

out:
    return res;
}

/**
 * @fn     - Validates the alignment of the heap and returns true if the alignments 
 *           fine or otherwise false
 * 
 * @return - true if the alignment's fine or otherwise false 
*/
static uint16_t heap_validate_alignement(void* ptr)
{
    return ((uint16_t) ptr % (OS_HEAP_BLOCK_SIZE)) == (ZERO);
}

/**
 * @fn     - The person who wants to create a heap will call heap create. 
 *           They will pass in an uninitialized heap structure pointer and 
 *           we will initialize it. They'll pass in where they want the heap
 *           to start, where they want the heap to end. They will also be
 *           responsible for providing a valid heap table.
 * 
 * 
 * @return - status of operation: 0 if succeeded, negative number otherwise
*/
uint16_t heap_create(Heap_t* heap, void* ptr, void* end, HeapTable_t* table)
{
    uint16_t res = (ZERO);

    // chekc alignment of the heap
    if (!heap_validate_alignement(ptr) || heap_validate_alignement(end))
    {
        res = -EINVARG;
        goto out;
    }

    memset(heap, ZERO, sizeof(Heap_t)); // initialize the heap with zeroes
    heap -> saddr = ptr;                // set the start address to the pointer we were provided
    heap -> table = table;

    res = heap_validate_table(ptr, end, table);
    if (res < ZERO)
    {
        goto out;
    }

    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table -> total; // 8 bits * total table entries
    memset(table -> entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);   // write entry free flag to all entries so all entries will be free at init

out: // handling errors like linux kernel
    return res;
}

/**
 * @fn   - returns number of bytes alligned to 4096 so
 *         the aligmnent stays consistent with our blocks
*/
static uint32_t heap_algn_value_to_upper(uint32_t val)
{
    // if we're already aligned, return the value
    if ((val % (OS_HEAP_BLOCK_SIZE)) == 0)
    {
        return val;
    }

    val = (val - (val % OS_HEAP_BLOCK_SIZE));
    val += OS_HEAP_BLOCK_SIZE;

    return val;
}

void* heap_malloc_blocks(Heap_t heap, uint32_t total_blocks)
{
    void* address = ZERO;
    uint16_t start_block = heap_get_start_block(heap, total_blocks);
    if (start_block < 0)
    {
        goto out;
    }

    address = heap_block_to_address(heap, start_block);

    // Mark the blocks as taken
    heap_mark_blocks_taken(heap, start_block, total_blocks);
    

out:
    return address;
}

/**
 * @fn   - calculates the number of bytes to write, according to
 *         heap block size and allocates the calculated number 
 *         of bytes in memory
 *         
*/
void* heap_malloc(Heap_t heap, size_t size)
{
    size_t aligned_size = heap_algn_value_to_upper(size);
    uint32_t total_blocks = aligned_size / OS_HEAP_BLOCK_SIZE;

    return heap_malloc_blocks(heap, total_blocks);
}

/**
 * @fn   - frees the given number of bytes in memory
*/
void* heap_malloc(Heap_t heap, void* ptr)
{

}