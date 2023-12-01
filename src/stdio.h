#ifndef STDIO_H
#define STDIO_H
#include <stdint.h>
#include <stddef.h>

void terminal_init();
void print(const uint8_t* str);
void terminal_writechar(uint8_t data, uint8_t color);
void terminal_putchar(uint8_t data, uint8_t color, uint8_t x, uint8_t y);
uint16_t terminal_make_char(uint8_t c, uint8_t colour);
size_t strlen(const uint8_t* str);
void append_newline();

#define ZERO                0
#define VIDEO_MEM_ADDR      0xB8000

#define VGA_WIDTH           80
#define VGA_HEIGHT          80

#define WHITE_SPACE         ' '
#define NEW_LINE            '\n'

#endif