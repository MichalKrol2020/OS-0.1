#include "../../inc/animations/animations.h"
#include "../../inc/graphics/graphics.h"
#include "../../inc/utils/utils.h"

static uint16_t get_gap(uint16_t start, uint16_t end) 
{
    if (start == end) {
        return (ZERO);
    } 
    
    else if (start > end)
    {
        return start - end;
    } 
    
    else
    {
        return end - start;
    }
}

void init_reposition(Animation_t* animation)
{
    // RepositionAnimationData_t* animation_details = (RepositionAnimationData_t*) animation -> animation_details;
    // InitialDetails_t* initial_details = animation -> initial_details;
    // uint16_t x_gap = get_gap(initial_details -> x_init_pos, animation_details -> x_end_pos);
    // uint16_t y_gap = get_gap(initial_details -> y_init_pos, animation_details -> y_end_pos);
    // uint8_t is_right = initial_details -> x_init_pos < animation_details -> x_end_pos;
    // uint8_t is_top = initial_details -> y_init_pos < animation_details -> y_end_pos;

    // TODO calc step
}

static uint16_t calc_pos(uint8_t standard_dir, uint16_t init_pos, uint16_t current_px)
{
    if (standard_dir) 
    {
        return init_pos + current_px + (ONE);
    } else 
    {
        return init_pos - current_px - (ONE);
    }

}

static RepositionData_t calc_reposition(Animation_t* animation)
{
    RepositionAnimationData_t* animation_details = (RepositionAnimationData_t*) animation -> animation_details;
    InitialDetails_t* initial_details = animation -> initial_details;

    uint16_t current_px = animation -> current_prog / animation_details -> step;
    uint16_t x_pos = calc_pos(animation_details -> right_dir, 
                              initial_details -> x_init_pos, current_px);

    uint16_t y_pos = calc_pos(animation_details -> top_dir, 
                              initial_details -> y_init_pos, current_px);

    RepositionData_t reposition_data = { x_pos, y_pos };
    return reposition_data;
}

void handle_reposition(Animation_t* animation)
{
    // RepositionAnimationData_t* animation_details = (RepositionAnimationData_t*) animation -> animation_details;
    // InitialDetails_t* initial_details = animation -> initial_details;
    // if ((animation -> current_prog % animation_details -> step) != (ZERO)) 
    // {
    //     animation -> current_prog++;
    //     return;
    // }

    // uint16_t x_pos, y_pos;
    // uint16_t current_px = animation -> current_prog / animation_details -> step;
    // RepositionData_t reposition_data = calc_reposition(animation);

}

// uint16_t calc_step(uint16_t duration_ms) {

// }