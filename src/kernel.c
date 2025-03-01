#include "../inc/stdio.h"
#include "../inc/io/io.h"
#include "../inc/kernel.h"
#include "../inc/idt/idt.h"
#include "../inc/timer/timer.h"
#include "../inc/startup/startup.h"
#include "../inc/memory/heap/kheap.h"
#include "../inc/graphics/graphics.h"
#include "../inc/memory/paging/paging.h"
#include "../inc/graphics/graphics-mem.h"
#include "../inc/collections/linked-list.h"

void kernel_main() 
{
    draw_logo();

    init_loading_bar();

    // Initialize the heap
    kheap_init();

    // Initialize the Interrupt Descriptor Tablse
    idt_init();

    timer_init(1000);

    // Enable the system interrupt
    enable_interrupts();


    Node_t* list = create_node();

    uint8_t a = 8;
    int8_t result = add((void*) &a, list);

    result = remove(list -> next, list);
    result = remove(list, list);
    if (result);
}