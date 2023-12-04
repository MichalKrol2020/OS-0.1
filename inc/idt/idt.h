#ifndef IDT_H
#define IDT_H
#include <stdint.h>

#define IDT_GATE_TYPE_INTERRUPT         0xEEu // 0xEE instead of 0x0E because we have to take into account DPL, Segment and Present bits
#define IDT_HIGHER_OFFSET_SHIFT_BITS    16    // we need to make a shift because we only want the higher bits of the offset

#define MASTER_PIC_PORT20               0x20
#define MASTER_PIC_PORT21               0x21
#define PIC_ACK_VALUE                   0x20
#define IRQ21_KEYBOARD_PRESS_NO         0x21

typedef struct __attribute__ ((packed))
{
    uint16_t offset_1;  // Offset bits 0 - 15               - Lower part of the interrupt function's offset address (pointer)
    uint16_t selector;  // Selector thats in out GDT        - Selector of the interrupt function (kernel's selector). THe selector's descriptor DPL field has to be 0 so the iret instruction won't throw a #GP exception hen executed
    uint8_t zero;       // Unused, set to zer               - Have to be 0
    uint8_t type_attr;  // Descriptor type and attributes   - IDT gaate type
    uint16_t offset_2;  // Offset bits 16 -31               - Higher part of the offset
} IDT_Desc_t;

typedef struct __attribute__ ((packed))
{
    uint16_t limit; // Size of descriptor table -1
    IDT_Desc_t* base;  // Base address of the start of the interrupt descriptor table
} IDTR_Desc_t;

void idt_init();
void enable_interrupts();
void disable_interrupts();
#endif