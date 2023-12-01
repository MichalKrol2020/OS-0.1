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
    or al,2
    out 0x92, al
    
    call kernel_main ; call our c function

    jmp $

times 512-($ - $$) db 0 ; align data so any assembly code wont affect our c kernel code. It has to be aligned to 16 bytes because that's what C compiler is using (512 % 16 = 0)
