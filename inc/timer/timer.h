#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define PIT_DIVISOR                     1193180 // PIT frequency is 1.1931816666 MHz
#define CHANNEL_0_DATA_PORT             0x40u
#define CHANNEL_1_DATA_PORT             0x41u
#define CHANNEL_2_DATA_PORT             0x42u
#define MODE_COMMAND_REGISTER_PORT      0x43u
#define MODE_COMMAND_VALUE              0x36u // 0011 0110 - (0 - 16-bit binary), (011 - square wave generator), (11 - Access mode: lobyte/hibyte), (00 - Channel 0)

void timer_init(uint32_t frequency);
void update_ticks();
void on_irq0();

#endif