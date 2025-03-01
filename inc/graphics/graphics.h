#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

#define VGA_MEMORY_ADDR         0xA0000
#define VGA_SCREEN_WIDTH        320
#define VGA_SCREEN_HEIGHT       200
#define VGA_PIXEL_COLOR_BITS    8

#define VGA_SIZE_BYTES          ((VGA_SCREEN_HEIGHT) * (VGA_SCREEN_WIDTH))

typedef struct
{
    uint16_t pos_x;
    uint16_t pos_y;
    uint16_t width;
    uint16_t height;
    uint8_t thickness;
    uint8_t vga_color;
} RectConfig_t;

void fill_rect(uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height, uint8_t vga_color);
void draw_img(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* pixels);
void draw_rect(RectConfig_t* config);

void putpixel(uint16_t pos_x, uint16_t pos_y, uint8_t vga_color);
void putpixel_db(uint16_t pos_x, uint16_t pos_y, uint8_t vga_color, void* buffer_b);

#endif