section .asm 

extern int21h_handler
extern no_interrupt_handler

global int21h
global idt_load
global enable_interrupts
global disable_interrupts
global no_interrupt

enable_interrupts: ; we don't need to do the stack frame because it's a very simple operation 
    sti
    ret

disable_interrupts:
    cli
    ret

idt_load:
    push ebp ; Push the current value of the base pointer (ebp) onto the stack. This is a standard part of function prologues to save the previous stack frame.
    mov ebp, esp ;  Set the base pointer (ebp) to the current stack pointer (esp). This establishes a new stack frame for the function

    mov ebx, [ebp+8] ; will move the value from ebp +8 into ebx, the value is the pointer. The +8 offset is commonly used for the first parameter on the stack.
    lidt [ebx] ; Load the Interrupt Descriptor Table (IDT) using the lidt (Load IDT) instruction. The operand [ebx] contains the address of the IDTR_Desc_t structure, which holds the base address and limit of the IDT.
    pop ebp ; Pop the saved base pointer value from the stack, restoring the previous stack frame.
    ret

init21h: ; we should always create interrupt handler wrapper in assembly to return from the interrupt properly
    cli
    pushad ; pushes EAX, ECX, EDX, EBX, original ESP, EBP, ESI and EDI - we need to save those registers because when we do iret, it expects those registers state to be the same
    call int21h_handler
    popad
    sti
    iret

no_interrupt: ; our PIC doesn't know if we implemented an interrupt or not and if there is no interrupt handler and the interrupt gets called then the processor will fall into a hard fault and reset
    cli
    pushad
    call no_interrupt_handler
    popad
    sti
    iret
