#include "../../inc/timer/timer.h"
#include "../../inc/utils/utils.h"
#include "../../inc/io/io.h"

static volatile uint64_t g_ticks = (ZERO);

void timer_init(uint32_t frequency)
{
    uint32_t divisor = (PIT_DIVISOR) / frequency;
    uint8_t divisor_lo = (uint8_t) (divisor & (BIT_MASK_8BIT));
    uint8_t divisor_hi = (uint8_t) ((divisor >> (BITS_8)) & (BIT_MASK_8BIT));

    outb((MODE_COMMAND_REGISTER_PORT), (MODE_COMMAND_VALUE));
    outb((CHANNEL_0_DATA_PORT), divisor_lo);
    outb((CHANNEL_0_DATA_PORT), divisor_hi);
}

void handle_timer()
{
    
}
