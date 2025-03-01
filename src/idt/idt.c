#include "../inc/startup/startup.h"
#include "../inc/memory/memory.h"
#include "../inc/utils/utils.h"
#include "../inc/idt/idt.h"
#include "../inc/config.h" 
#include "../inc/io/io.h"
#include "../inc/timer/timer.h"
#include "../inc/graphics/graphics.h"

IDT_Desc_t idt_descriptors[OS_TOTAL_INTERRUPTS];
IDTR_Desc_t idtr_descriptor;

extern void idt_load(IDTR_Desc_t* idtr_ptr);
extern void int20h();
extern void int21h();
extern void no_interrupt();

void int20h_handler()
{
    outb((MASTER_PIC_PORT20), (PIC_ACK_VALUE));
}

void int21h_handler() 
{
    
    outb((MASTER_PIC_PORT20), (PIC_ACK_VALUE));
}

void no_interrupt_handler()
{
    outb(MASTER_PIC_PORT20, PIC_ACK_VALUE); // just send the ack value so the processor will know that the interrupt has handled
}

void idt_set(uint16_t interrupt_no, void* address)
{
    IDT_Desc_t* const desc = &idt_descriptors[interrupt_no];
    desc -> offset_1 = (uint32_t) address & (MAX_16BIT_VALUE_HEX); // sets the lower part of the offset
    desc -> selector = (KERNEL_CODE_SELECTOR); 
    desc -> zero = (ZERO_HEX);
    desc -> type_attr = (IDT_GATE_TYPE_INTERRUPT);
    desc -> offset_2 = (uint32_t) address >> (IDT_HIGHER_OFFSET_SHIFT_BITS);
}

void idt_init() 
{
    memset((void*) idt_descriptors, ZERO, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) - (ONE); // The limit defines the length of the IDT in bytes -1
    idtr_descriptor.base = (IDT_Desc_t*) idt_descriptors;   // The linear address where the IDT starts (INT 0)

    for (uint16_t i = (ZERO); i < OS_TOTAL_INTERRUPTS; i++)
    {
        idt_set(i, no_interrupt);
    }

    idt_set((IRQ0_PIT_TICK_NO), int20h);
    idt_set((IRQ21_KEYBOARD_PRESS_NO), int21h);

    // Load the interrupt descriptor table
    idt_load(&idtr_descriptor);

    update_progress((PROGRESS_IDT_FINISHED));
}