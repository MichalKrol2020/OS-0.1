#include "../inc/stdio.h"


uint8_t terminal_row = 0;
uint8_t terminal_col = 0;

void terminal_init() 
{
    terminal_row = 0;
    terminal_col = 0;
    for (int y = (ZERO); y < (VGA_HEIGHT); y++) 
    {
        for (int x = (ZERO); x < (VGA_WIDTH); x++) 
        {
            terminal_putchar((WHITE_SPACE), (ZERO), x, y);
        }
    }
}

void print(const uint8_t* const str) 
{
    size_t len = strlen(str);
    for (uint8_t i = ZERO; i < len; i++) 
    {
        terminal_writechar(str[i], 15);
    }
}

void terminal_writechar(uint8_t data, uint8_t color) 
{
    if (data == (NEW_LINE)) 
    {
        append_newline();
        return;
    }

    if (terminal_col >= VGA_WIDTH) 
    {
        append_newline();
    }

    uint8_t x = terminal_col;
    uint8_t y = terminal_row;

    terminal_putchar(data, color, x, y);
    terminal_col++;
}

void terminal_putchar(uint8_t data, uint8_t color, uint8_t x, uint8_t y) 
{
    uint16_t* const video_mem = (uint16_t*) (VIDEO_MEM_ADDR);
    video_mem[(y * (VGA_WIDTH)) + x] = terminal_make_char(data, color);
}

uint16_t terminal_make_char(uint8_t c, uint8_t colour) 
{
    return (colour << 8) | c;
}

size_t strlen(const uint8_t* const str)
{
    size_t len = ZERO;
    while(str[len])
    {
        len++;
    }

    return len;
}

void append_newline()
{
    terminal_col = (ZERO);
    terminal_row++;
}