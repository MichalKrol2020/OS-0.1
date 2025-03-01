section .asm

global memcpy

memcpy:
    cli
    push ebp
    mov ebp, esp
    mov edi, [ebp + 8]  ; Get the second argument (src)
    mov esi, [ebp + 12] ; Get the first argument (dest)
    mov ecx, [ebp + 16]  ; Get the third argument (count)

    rep movsb           ; Copy bytes from esi to 
    pop ebp
    sti 

    ret