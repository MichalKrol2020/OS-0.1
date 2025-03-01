[BITS 32]

global _start ; exports the symbpl because otherwise when we assemble this to an object file, this will not be publicly known.

extern kernel_main

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:             
    mov ax, DATA_SEG ; setting the data registers
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000 ; setting the stack pointer further in memory
    mov esp, ebp

    ; Enable A20 Line
    in al, 0x92
    or al, 2
    out 0x92, al

    ; remap of the master PIC. We have to do a remap because first numbers are taken by intel exceptions so we need to set our interrupts at the starting number 0x20
    mov al, 00010001b ; put the PIC into initialization mode
    out 0x20, al ; write to port 0x20 (master PIC)

    mov al, 0x20 ; Interrupt 0x20 is where master ISR should start (right after Intels exceptions)
    out 0x21, al

    mov al, 00000001b ; put PIC in x86 mode
    out 0x21, al

    ; Remap the slave PIC
    mov al, 00010001b ; Initialize slave PIC
    out 0xA0, al

    mov al, 0x28 ; Set slave PIC vector offset to 0x28
    out 0xA1, al

    mov al, 00000010b ; Tell slave PIC its cascade identity is 2
    out 0xA1, al

    mov al, 00000101b ; Set slave PIC to 8086 mode
    out 0xA1, al

    call kernel_main ; call our c function

    jmp $

times 512-($ - $$) db 0 ; align data so any assembly code wont affect our c kernel code. It has to be aligned to 16 bytes because that's what C compiler is using (512 % 16 = 0)
