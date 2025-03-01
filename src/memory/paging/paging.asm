[BITS 32]

section .asm

global paging_load_directory
global enable_paging

PAGING_ENABLE_BITMASK equ 0x80000001

; Enabling paging is actually very simple. 
; All that is needed is to load CR3 with the 
; address of the page directory and to set the 
; paging (PG) and protection (PE) bits of CR0.
;
paging_load_directory:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr3, eax
    pop ebp

    ret

enable_paging:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or eax, PAGING_ENABLE_BITMASK
    mov cr0, eax
    pop ebp

    ret