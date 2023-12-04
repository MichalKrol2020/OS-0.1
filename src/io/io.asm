section .asm

global insb
global insw
global outb
global outw

insb:
    push ebp ; create stack frame (good practice)
    mov ebp, esp ; move our argument variable into ebp from our stack
    
    xor eax, eax ; clear eax register
    mov edx, [ebp+8] ; pass the argument varable from c function call to dx register
    in al, dx ; input byte from I\O port in DX into Al. The lower 8 bits of eax will be set to the byte loaded from the port proided to us

    pop ebp
    ret ; eax register is always the return value so we will get the read data in return

insw:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov edx, [ebp+8]
    in ax, dx

    pop ebp
    ret

outb:
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, al

    pop ebp
    ret

outw:
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, ax

    pop ebp
    ret


