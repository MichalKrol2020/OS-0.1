#include "../../../inc/memory/heap/heap.h"
#include "../../../inc/memory/memory.h"
#include "../../../inc/utils/utils.h"
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
static int16_t heap_validate_table(void* ptr, void* end, HeapTable_t* table)
{
    int16_t res = ZERO;
    size_t table_size = (size_t)(end - ptr);
    size_t total_blocks = table_size / (OS_HEAP_BLOCK_SIZE);
    if (table -> total != total_blocks)
    {
        res = -(EINVARG);
        goto out;
    }

out:
    return res;
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
int16_t heap_create(Heap_t* heap, void* ptr, void* end, HeapTable_t* table)
{
    int16_t res = (ZERO);

    // chekc alignment of the heap
    if (!validate_alignement(ptr, (OS_HEAP_BLOCK_SIZE)) || 
        !validate_alignement(end, (OS_HEAP_BLOCK_SIZE)))
    {
        res = -(EINVARG);
        goto out;
    }

    memset(heap, (ZERO), sizeof(Heap_t)); // initialize the heap with zeroes
    heap -> saddr = ptr;                  // set the start address to the pointer we were provided
    heap -> table = table;

    res = heap_validate_table(ptr, end, table);
    if (res < (ZERO))
    {
        goto out;
    }

    size_t table_size = sizeof(uint8_t) * table -> total; // 8 bits * total table entries
    memset(table -> entries, (HEAP_BLOCK_TABLE_ENTRY_FREE), table_size);   // write entry free flag to all entries so all entries will be free at init

out: // handling errors like linux kernel
    return res;
}

/**
 * @fn   - returns number of bytes alligned to 4096 so
 *         the aligmnent stays consistent with our blocks
*/
static uint32_t heap_align_value_to_upper(uint32_t val)
{
    // if we're already aligned, return the value
    if ((val % (OS_HEAP_BLOCK_SIZE)) == (ZERO))
    {
        return val;
    }

    val = (val - (val % (OS_HEAP_BLOCK_SIZE)));
    val += (OS_HEAP_BLOCK_SIZE);

    return val;
}

static int16_t heap_get_entry_type(uint8_t entry)
{
    return entry & (BIT_MASK_4BIT); // get the last 4 bits
}

int16_t heap_get_start_block(Heap_t* heap, uint32_t total_blocks)
{
    HeapTable_t* table = heap -> table;
    uint16_t current_block = (ZERO); // stores the current block
    int16_t start_block =  (MINUS_ONE);     // first free block that we find

    for (size_t i = (ZERO); i < table -> total; i++)
    {
        // if the entry we're on is not free, then we need to reset the state because there's no way we can use our start entry that we already have.
        // Assuming there is a start entry, let's say it finds one block, then BS might be equal to five or something
        // Well, we haven't reached the total block so that we can't use this start block.
        // So we reset BS to minus one and we reset the current block to zero here.
        if (heap_get_entry_type(table -> entries[i]) != (HEAP_BLOCK_TABLE_ENTRY_FREE))
        {
            current_block = (ZERO);
            start_block = (MINUS_ONE);
            continue;
        }

        // we just need to find the starting block because we already know how much more blocks we will need.
        // And these blocks will be right after the start block. That is why we search the start block
        // and reset it whenever there is not enough other blocks after it. 
        if (start_block == (MINUS_ONE))
        {
            start_block = i; // now we know our start block
        }

        // If there is enough blocks after our starting block then we have found our memory blocks to allocate
        current_block++;
        if (current_block == total_blocks) // we found our memory block
        {
            break;
        }
    }

    if (start_block == (MINUS_ONE))
    {
        return -(ENOMEM);        
    }

    return start_block;
}

void* heap_block_to_address(Heap_t* heap, uint16_t blocks)
{
    return heap -> saddr + (blocks * (OS_HEAP_BLOCK_SIZE)); // we know where the heap start (saddr), we know how to calculate the offset because we know that each block handles 4096 byte. So we need to multiply blocks by 4096 and then we add our offset to the start address of the heap
}

void heap_mark_blocks_taken(Heap_t* heap, int16_t start_block, int16_t total_blocks)
{
    int16_t end_block = (start_block + total_blocks) - (ONE); // we count from 0 so -1
    uint8_t entry = (HEAP_BLOCK_TABLE_ENTRY_TAKEN) | (HEAP_BLOCK_IS_FIRST); // set it to taken and is_first  
    if (total_blocks > (ONE)) // if there are more blocks we need to mark entry as has next
    {
        entry |= (HEAP_BLOCK_HAS_NEXT);
    }

    for (int16_t i = start_block; i <= end_block; i++)
    {
        heap -> table -> entries[i] = entry;
        entry = (HEAP_BLOCK_TABLE_ENTRY_TAKEN); // set the entry to taken because we don't need is_first flag anymore as it is not the first block  
        if (i != end_block - (ONE))
        {
            entry |= (HEAP_BLOCK_HAS_NEXT);
        }
    }
}

void heap_mark_blocks_free(Heap_t* heap, int16_t start_block)
{
    HeapTable_t* table = heap -> table;
    int16_t end_block = (start_block + table -> total) - (ONE); // we count from 0 so -1
    for (int16_t i = start_block; i <= end_block; i++)
    {
        table -> entries[i] = (HEAP_BLOCK_TABLE_ENTRY_FREE);
        if (!(table -> entries[i] & (HEAP_BLOCK_HAS_NEXT)))
        {
            break;
        }
    }
}

void* heap_malloc_blocks(Heap_t* heap, uint32_t total_blocks)
{
    void* address = (ZERO);
    int16_t start_block = heap_get_start_block(heap, total_blocks);
    if (start_block < (ZERO))
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
void* heap_malloc(Heap_t* heap, size_t size)
{
    size_t aligned_size = heap_align_value_to_upper(size);
    uint32_t total_blocks = aligned_size / OS_HEAP_BLOCK_SIZE;

    return heap_malloc_blocks(heap, total_blocks);
}

int16_t heap_address_to_block(Heap_t* heap, void* address) 
{
    return (address - heap -> saddr) / (OS_HEAP_BLOCK_SIZE); // address - heap -> saddr  - to get the number of bytes allocated
}

/**
 * @fn   - frees the given number of bytes in memory
*/
void heap_free(Heap_t* heap, void* ptr)
{
    heap_mark_blocks_free(heap, heap_address_to_block(heap, ptr));
}