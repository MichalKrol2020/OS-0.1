#ifndef LOGO_H
#define LOGO_H

#include <stdint.h>

#define LOGO_X_OFFSET              (((VGA_SCREEN_WIDTH) - (LOGO_WIDTH)) / 2)
#define LOGO_Y_OFFSET              ((((VGA_SCREEN_HEIGHT) - (LOGO_HEIGHT)) / 2) - 30)
#define LOGO_HEIGHT                75
#define LOGO_WIDTH                 75
#define PROGRESS_STEP              10
#define LOADING_BAR_MAX_BARS       10
#define LOADING_BAR_MAX_PROGRESS   100

#define LOADING_BAR_X_POS          110
#define LOADING_BAR_Y_POS          140
#define LOADING_BAR_HEIGHT         20
#define LOADING_BAR_WIDTH          104
#define LOADING_BAR_THICKNESS      1
#define LOADING_BAR_COLOR_VGA      0x0fu
#define LOADING_BAR_SEPAR_X        2
#define LOADING_BAR_SEPAR_Y        2

#define LOADING_BAR_SEGMENT_X_POS  ((LOADING_BAR_X_POS) + (LOADING_BAR_THICKNESS) + (LOADING_BAR_SEPAR_X))
#define LOADING_BAR_SEGMENT_Y_POS  ((LOADING_BAR_Y_POS) + (LOADING_BAR_THICKNESS) + (LOADING_BAR_SEPAR_Y))
#define LOADING_BAR_SEGMENT_HEIGHT ((LOADING_BAR_HEIGHT) - 2 * (LOADING_BAR_THICKNESS) - 2 * (LOADING_BAR_SEPAR_Y))
#define LOADING_BAR_SEGMENT_WIDTH  8

#define PROGRESS_KHEAP_FINISHED    50
#define PROGRESS_IDT_FINISHED      100

extern uint8_t logo_data[];

void draw_logo();
void init_loading_bar();
void update_progress(uint8_t value);

#endif