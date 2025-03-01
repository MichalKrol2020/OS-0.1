#include "../../inc/graphics/graphics.h"
#include "../../inc/utils/utils.h"

static void draw_top_horiz(RectConfig_t* config) {
    fill_rect(config -> pos_x, 
              config -> pos_y, 
              config -> width, 
              config -> thickness, 
              config -> vga_color);
}

static void draw_bottom_horiz(RectConfig_t* config) {
    fill_rect(config -> pos_x + config -> thickness,
              config -> pos_y + config -> height - config -> thickness,
              config -> width - config -> thickness,
              config -> thickness,
              config -> vga_color);
}

static void draw_left_vert(RectConfig_t* config) {
    fill_rect(config -> pos_x,
              config -> pos_y + config -> thickness,
              config -> thickness,
              config -> height - config -> thickness,
              config -> vga_color);
}

static void draw_right_vert(RectConfig_t* config) {
    fill_rect(config -> pos_x + config -> width - config -> thickness,
              config -> pos_y + config -> thickness,
              config -> thickness,
              config -> height - (config -> thickness * 2),
              config -> vga_color);
}

void putpixel(uint16_t pos_x, uint16_t pos_y, uint8_t vga_color)
{
    uint8_t* location = (uint8_t*)(VGA_MEMORY_ADDR) + (VGA_SCREEN_WIDTH) * pos_y + pos_x;
    *location = vga_color;
}

void putpixel_db(uint16_t pos_x, uint16_t pos_y, uint8_t vga_color, void* buffer_b)
{
    uint8_t* location = (uint8_t*)(buffer_b) + (VGA_SCREEN_WIDTH) * pos_y + pos_x;
    *location = vga_color;
}

void fill_rect(uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height, uint8_t vga_color) {
    for (uint16_t i = (ZERO); i < width; i++) {
        for (uint16_t j = (ZERO); j < height; j++) {
            uint16_t x_offset = i + pos_x;
            uint16_t y_offset = j + pos_y;
            putpixel(x_offset, y_offset, vga_color);
        }
    }
}

void draw_img(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* pixels) {
    uint16_t current_index = (ZERO);
    for (uint16_t l = (ZERO); l < h; l++) {
        for (uint16_t i = (ZERO); i < w; i++) {
            current_index++;
            putpixel(x + i, y + l, pixels[current_index]);
        }
    }
}

void draw_rect(RectConfig_t* config) {
    draw_top_horiz(config);
    draw_bottom_horiz(config);
    draw_left_vert(config);
    draw_right_vert(config);
}