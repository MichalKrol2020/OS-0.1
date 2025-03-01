#include "../../inc/graphics/graphics-mem.h"
#include "../../inc/memory/heap/kheap.h"
#include "../../inc/graphics/graphics.h"
#include "../../inc/memory/memory.h"
#include "../../inc/utils/utils.h"

void* clone_vga_buffer()
{
    void* vga_ptr = (void*) (VGA_MEMORY_ADDR);
    void* buffer_B = kmalloc(VGA_SIZE_BYTES);
    memcpy(buffer_B, vga_ptr, (VGA_SIZE_BYTES));
    
    return buffer_B;
}

void write_vga(void* buffer_B)
{
    void* vga_ptr = (void*) (VGA_MEMORY_ADDR);
    memcpy(vga_ptr, buffer_B, (VGA_SIZE_BYTES));
}