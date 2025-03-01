#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <stdint.h>

typedef struct
{
    uint16_t x_pos;
    uint16_t y_pos;
} RepositionData_t;

typedef struct
{
    uint16_t duration_ms;
} EaseInAnimationData_t;

typedef struct
{
    uint16_t duration_ms;
    uint16_t x_end_pos;
    uint16_t y_end_pos;
    uint16_t step;
    uint8_t right_dir;
    uint8_t top_dir;
} RepositionAnimationData_t;

typedef union
{
    EaseInAnimationData_t ease_in_data;
    RepositionAnimationData_t reposition_data;
} AnimationDetails_u;

typedef struct 
{
    uint16_t x_init_pos;
    uint16_t y_init_pos;
    uint16_t width;
    uint16_t height;
} InitialDetails_t;

typedef struct
{
    uint8_t* data;
    uint16_t current_prog;
    AnimationDetails_u* animation_details;
    InitialDetails_t* initial_details;
    void* handler;
} Animation_t;


#endif