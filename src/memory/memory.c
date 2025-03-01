#include "../inc/memory/memory.h"
#include "../inc/utils/utils.h"

uint8_t validate_alignement(void* ptr, size_t align_bytes)
{
    return ((uint32_t) ptr % align_bytes) == (ZERO);
}

void* memset(void* ptr, uint16_t c, size_t size) 
{
    uint8_t* c_ptr = (uint8_t*) ptr;
    for (uint16_t i = (ZERO); i < size; i++)
    {
        c_ptr[i] = (uint8_t) c;
    }

    return ptr;
}