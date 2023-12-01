#include "kernel.h"
#include "stdio.h"

void kernel_main() 
{
    terminal_init();
    const uint8_t* data =(uint8_t*) "Hello there!\nHello from the other line!";
    print((uint8_t*) data);
}