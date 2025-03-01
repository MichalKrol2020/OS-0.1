#include "../../inc/graphics/graphics.h"
#include "../../inc/startup/startup.h"
#include "../../inc/utils/utils.h"

static uint8_t progress = (ZERO);

static void append_bars(uint8_t count) 
{
    uint8_t y_pos = (LOADING_BAR_SEGMENT_Y_POS);
    uint8_t current_bars = progress / (PROGRESS_STEP);
    uint8_t current_separ_width = current_bars * (LOADING_BAR_SEPAR_X);
    uint8_t current_bars_width = current_bars * (LOADING_BAR_SEGMENT_WIDTH);
    uint8_t current_x_pos = (LOADING_BAR_SEGMENT_X_POS) + current_bars_width + current_separ_width;
    for (uint8_t i = (ZERO); i < count; i++)
    {
        fill_rect(current_x_pos, y_pos, (LOADING_BAR_SEGMENT_WIDTH), (LOADING_BAR_SEGMENT_HEIGHT), (LOADING_BAR_COLOR_VGA));
        current_x_pos += (LOADING_BAR_SEGMENT_WIDTH) + (LOADING_BAR_SEPAR_X);
    }
}

static uint8_t count_bars_to_draw(uint8_t value)
{
    uint8_t current_bars = progress / (PROGRESS_STEP);
    uint8_t updated_bars = (progress + value) / (PROGRESS_STEP);
    if (updated_bars > (LOADING_BAR_MAX_BARS))
    {
        return (LOADING_BAR_MAX_BARS) - current_bars; 
    }

    return updated_bars - current_bars;
}

void draw_logo()
{
    draw_img((LOGO_X_OFFSET), (LOGO_Y_OFFSET), (LOGO_WIDTH), (LOGO_HEIGHT), logo_data);
    
}

void init_loading_bar()
{
    RectConfig_t rectConfig;
    rectConfig.pos_x     = (LOADING_BAR_X_POS);
    rectConfig.pos_y     = (LOADING_BAR_Y_POS);
    rectConfig.width     = (LOADING_BAR_WIDTH);
    rectConfig.height    = (LOADING_BAR_HEIGHT);
    rectConfig.thickness = (LOADING_BAR_THICKNESS);
    rectConfig.vga_color = (LOADING_BAR_COLOR_VGA);

    draw_rect(&rectConfig);
}

void update_progress(uint8_t value)
{
    uint8_t to_draw = count_bars_to_draw(value);

    if (to_draw > (ZERO))
    {
        append_bars(to_draw);
    }

    uint8_t updated_progress = progress + value;
    progress = updated_progress > (LOADING_BAR_MAX_PROGRESS) ? 
               (LOADING_BAR_MAX_PROGRESS) : updated_progress;
}
