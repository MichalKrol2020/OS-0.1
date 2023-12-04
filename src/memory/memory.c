#include "../inc/memory/memory.h"

void* memset(void* ptr, uint16_t c, size_t size) 
{
    uint8_t* c_ptr = (uint8_t*) ptr;
    for (uint16_t i = 0; i < size; i++)
    {
        c_ptr[i] = (uint8_t) c;
    }

    return ptr;
}