#include "../inc/kernel.h"
#include "../inc/stdio.h"
#include "../inc/idt/idt.h"
#include "../inc/io/io.h"



void kernel_main() 
{
    terminal_init();
    const uint8_t* data =(uint8_t*) "Hello there!\nHello from the other line!";
    print((uint8_t*) data);

    // Initialize the Interrupt Descriptor Tablse
    idt_init();

    // Enable the system interrupt
    enable_interrupts();
}