#ifndef GRAPHICS_MEM_h
#define GRAPHICS_MEM_h

#include <stdint.h>

void* clone_vga_buffer();
void write_vga(void* buffer_B);

#endif