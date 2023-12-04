ORG 0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short start
    nop ; Bios parameter block requires us to do the nop at the first 3 bytes of code

times 33 db 0 ; fill 33 bytes of BIOS prameter block so the BIOS doesn't overwrite it

start:
    jmp 0:step2 ; change code segment to 0x7c0

step2:
    cli ; clear interrupts
    mov ax, 0x00
    mov ds, ax ; by changing thses segment registers ds and es we're taking control of the sitution rather than hoping that bios will load the value
    mov es, ax ; es - extra segment
    mov ss, ax
    mov sp, 0x7c00 ; set the stack pointer to point to the absolute position of the first instruction (0x7c0 * 16 = 0x7c00) 
    sti ; enable interrupts

.load_protected:
    cli 
    lgdt[gdt_dscriptor] ; loads gdt (load global descriptor table)
    mov eax, cr0        ; in order to change the last bit of cr0 to 1 we have to use one of the general purpose registers (eax, ebx, ecx, edx)
    or eax, 0x1
    mov cr0, eax ; reset the CONTROL register
    jmp CODE_SEG:load32 ; code seg gets replaced with our gdt code offset 0x8. It switches to the code selector and it jumps to the load32 absolute address (to the 32 bit protected code )
    jmp $

; GDT
gdt_start:

gdt_null:  ; 64 bits of zeros - The null descriptor is 8 bytes wide
    dd 0x0 ; dd is a "pseudo-instruction" that assembles 4-byte constants into the output, the same way that add eax,eax assembles 0x01 0xc0 into the output.
    dd 0x0
; Some use null descriptor to store a pointer to the GDT itself (to use with the LGDT instruction). The null descriptor is 8 bytes wide and the pointer is 6 bytes wide so it might just be the perfect place for this.

; offset 0x8
gdt_code: ; cs should point to this
    dw 0xffff ; segment limit, first 0 - 15 bits 
    dw 0      ; base first 0 - 15 bits. The base of a segment is a 32-bit property that describes  the starting location of our segment. 
    db 0      ; base 16 - 23 bits. Limit is a 20-bit property.
    db 0x9a   ; access byte (flags) - osdev wiki. Access Byte - Specifies the type and privilege level of the segment. 9a in hexadecimal corresponds to 10011010 in binary, which indicates a code segment, readable, not expand-down, and with a privilege level of 0.
    db 11001111b ; high 4 bit flags and the low 4 bits flags. The high 4 bits are flags, and the low 4 bits are additional flags. In this case, 1100 corresponds to the limit's upper 4 bits.
    db 0      ; Base (24-32 bits) - Additional bits for the starting address.

; High 4 bits: 1100:
;
; Bit 0 (G): Granularity. If set, the limit is given in 4 KB blocks; otherwise, it's in bytes.
; Bit 1 (D/B): Default operation size. If set, it's a 32-bit segment; otherwise, it's a 16-bit segment.
; Bit 2 (L): Long mode. This bit is only used in 64-bit mode; in 32-bit mode, it should be 0.
; Bit 3 (AVL): Available for use by system software.
;
; Low 4 bits: 1111:
;
; Bit 0 (P): Segment present. If set, the segment is present in memory.
; Bit 1-3 (DPL): Descriptor privilege level. It represents the privilege level of the segment. 
; In this case, 111 means it's at the highest privilege level (Ring 0), which is typically used for kernel code.
;
; So, 11001111b in the context of a GDT code segment entry indicates a 32-bit code segment with granularity, long mode disabled, available for system software use, present in memory, and at the highest privilege level (Ring 0).

; offset 0x10
gdt_data:     ; load DS, ES, FS, GS, SS, ESP
    dw 0xffff ; segemnt limit, first 0 - 15 bits 
    dw 0      ; base first 0 - 15 bits
    db 0      ; base 16 - 23 bits
    db 0x92   ; access byte (flags) - osdev wiki
    db 11001111b ; high 4 bit flags and the low 4 bits flags
    db 0      ; base 24 - 32 bits

gdt_end:

gdt_dscriptor: ; lgdt will look down here, it will find the size (gdt_end - gdt_start - 1) and the offset (gdt_start)
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
load32:
    mov eax, 1 ; we use eax to represent the starting sector we want to load from. We don't want to load from 0 because that's the boot sector
    mov ecx, 100 ; 100 because we wrote nulls to 100 sectors in our Makefile. This will contain the total numbers of sectors we want to load
    mov edi, 0x0100000 ; equivalent of 1Meg, will hold the address we want to load the sectors into
    call ata_lba_read
    jmp CODE_SEG: 0x0100000

ata_lba_read:
    mov ebx, eax, ; Backup the LBA
    ; Send the highest 8 bits of the lba to hard disk controller
    shr eax, 24 ; shift the register 24 bits to the right so that after that instruction, the eax will contain highest 8 bits of the lba (32 - 24 = 8)
    or eax, 0xE0 ; selects the master drive
    mov dx, 0x1F6 ; the port that expects us to write these eight bits to
    out dx, al ; al is 8 bits and contains our highest 8 bits of the LBA. We output the data to the 0x1F6 port
    ; Finished sending the highest 8 bits of the LBA

    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al ; with out instruction we're talking with the bus on the motherboard
    ; Finished sending the total sectors to read

    ; Send more bits of the LBA
    mov eax, ebx ; Restore the backup LBA
    mov dx, 0x1F3
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send more bits of the LBA
    mov dx, 0x1F4
    mov eax, ebx ; Restore the backup LBA in case something bad happen when we edit it
    shr eax, 8
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send upper 16 - bits o the LBA
    mov dx, 0x1F5
    mov eax, ebx ; Restore the backup LBA
    shr eax, 16
    out dx, al
    ; Finished sending upper 16 - bits o the LBA

    mov dx, 0x1F7
    mov al, 0x20
    out dx, al

; Read all sectors into memory
.next_sector:
    push ecx

; Check if we need to read, because sometimes the controller has a bit of delay
.try_again:
    mov dx, 0x1f7
    in al, dx ; read from port 0x1f7
    test al, 8 ; test the al register if the bit is set in this bit mask so we look for 8
    jz .try_again ; if test failes - retry

; We need to rea 256 words at a tim
    mov ecx, 256 ; here we specify - do the insw instruction 256 times (that uses the ecx register)
    mov dx, 0x1F0
    rep insw ; Inputs word from I/O port specified in DX in to memory location specified in ES:(E)DI - that's we've specified mov edi, 0x0100000 at the beginning
    pop ecx ; restores the ecx which we,ve pushed before. in ecx there iis a total number of sectors we want to read
    loop .next_sector ; Decrements ecx at each iteration so we read the sectors from 100 to 0
    ; End of reading sectors into memory
    ret

times 510-($ - $$) db 0
dw 0xAA55