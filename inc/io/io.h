#ifndef IO_H
#define IO_H
#include <stdint.h>

uint8_t insb(uint16_t port); // read byte from the given port
uint16_t insw(uint16_t port); // read word from the given port

void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);

#endif