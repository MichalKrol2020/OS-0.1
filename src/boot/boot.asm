; CALCULATING ABSOLUTE OFFSET
; 1) Take the segment register,multiply it by 16 and add the offset:
;    
;    Code segment = 0x7c0
;    Assembly origin "org"is 0
;    Our first instruction is at origin 0 so our offset is 0
;    (0x7c0 * 16) = 0x7c00
;    (0x7c00 + 0) = 0x7c00
;
; DIFFERENT INSTRUCTIONS USER DIFFERENT SEGMENT REGISTERS
;
; 1) lodsb - uses the ds:si register combination (data segment as segment register and si as offset register),
;
; org 0 <- offset is 0 so the assembler will treat our labels as offsetting from 0
; mov ax, 0x7c0
; mov ds, ax
; mov si, 0x1f <- in real world this would be a assembly label pointing to a message
; lodsb <- loads a byte from the address pointed to by the si (source index) register
; 
; What happens here is the processor takes our data segment value which is 0x7c0
; and multiplies it by 16 to give us 0x7c00. 
;
; We then add the si register which is 0x1f. lodsb now knows that the byte it needs to 
; read is at address 0x7c1f
;
;
; INTERRUPTS
;
; 1) We have 256 interrupts handlers and each entry in vector table is 4 bytes.
;    The first 2 bytes of the value is offset in memory. 
;    The next 2 bytes is the segemtn in memory.
;
; 2) Interrupt vector table starts at absolute address zero in Ram. This is the very
;    first byte in memory where the interrupt vector table starts.
;
; 3) Combining the Offset and the Segment gives us the absolute address of where the 
;    interrupt should be run:
;
;    1st interrupt:
;    Offset: 0x00
;    Segment:0x7c0
;
;    What offset is interrupt 0x13?
;    0x13 * 0x04 = 0x46
;    Processor then extracts the ofsset from the interrupt, calculates the absolute address
;    and then executes it. To finish the interrupt we need to call the iret instruction.
;
; FILESYSTEM
;
; 1) Files does not directly exist on disk. They are rather a kernel operating system concept. 
;    On discs we have what's known as file systems. They are a special data structures that
;    explain files. It is up to the kernel to read these data structures correctly.
;
; 2) The disk itself has no concept of files. It just holds loads of data blocks called sectors.
;    If we want our kernel to understand files, we have to create a file system driver that targets 
;    that specific file system that is on our disc.
;
; 3) LBA - logical block address is a modern way of reading from a hard disk. Rather than 
;    specifying head track and sector we just specify a number that starts from 0.
;
;    So for example if we read LBA 0, then it would return the first sector on the disk to us.
;    LBA allows us to read from the disk as if we're reading blocks from a very large file.
;
;    To read from the disc we need to first calculate what sector number is for that offset
;    and then we need to read that sector and then offset from there to get the byte we're 
;    looking for.
;
; 4) Calculating LBA:
;
;    Let's say we want to read the byte at position 58376 in the disk. 
;    
;    - We get our LBA which is 58376 divided by 512 (sector size of almost all modern hard disks),
;    - We read that lba and we can load 512 bytes into memory,
;    - Next we need to know our offset that our byte is in our buffer,
;    - To calculate that offset we can do 58376 % 512 = 8 bytes into our buffer (that is where our byte is),
;
; 5) BIOS DISK ROUTINES
;
;    - In 16 bit real mode the bios provides interrupt 13th for disk operations. That is why we don't need
;      to write our own driver for reading files. We can just ask bios for that,
;    - In 32 bit protected mode we have to create our own disk driver
;
; 6) Reading from the hard disk
; 
; AH = 02h
; AL = number of sectors to read (must be nonzero)
; CH = low eight bits of cylinder number
; CL = sector number 1-63 (bits 0-5) high two bits of cylinder (bits 6-7, hard disk only)
; DH = head number
; DL = drive number (bit 7 set for hard disk) - when the BIOS loads our boot sector, 
;                                               the ds register is already set to the
;                                               drive number automatically, so we don't 
;                                               need to set this.It is automatically set
;                                               to the number of drive that we're booting
;                                               from
; ES:BX -> data buffer - the data is read into the extra segment, and bx is beeing the offset.
;                        It will be extra segment multiplied by 16 plus bx and thats where our
;                        data will be.
;
; Return:
; CF set on error
; if AH = 11h (corrected ECC error), AL = burst length
; CF clear if successful
; AH = status (see #00234)
; AL = number of sectors transferred (only valid if CF set for some BIOSes)
;
;
; == PROTECTED MODE ==
;
; 1) Protected mode is a processor state in x86 architecture which gives access to memory protection,
;    4GB address space and much more,
; 2) We have different protection lvls in our processor:
;    - ring 0: most privileged ring (kernel),
;    - ring 1,2: generally not used, sometimes used for device drivers,
;    - ring 3: the least privileged,prevents user programs from unprivileged acces (e.g. from clearing interrupts),
;    
; 3) User programs cause an interrupt, and by that the process can put itself back into ring 0, so that it
;    can begin running interrupt code on lvl 0. It's kernel responsibility to run into ring 3 and continue running
;    the user program. 
;
; 4) Different memory schemes in protected mode:
;    - Segment registers become selector registers,
;    - The other memory scheme is paging. This allows us to remap memory addresses so address 0x00 actually 
;      ponits to some other address for example
;
; 5) Selector memory scheme:
;    - selectors points to data structures that describe memory ranges and the permissions (ring levels) 
;      required to access a given range.
;
; 6) Paging memory scheme:
;    - We have virtual addresses and we can point them to physical addresses,
;    - Memory protection is easier to control,
;    - Paging is the most popular choice for memory schemes wuth kernels/os
;
;    For example, we have address 0x00 in the page directory. That points to the page table 0 but it points 
;    to completely different address in real memory. This allows all programs to believe that they are loaded
;    at the same address. It also allows us to comlpetely map out all of the user programs in memory so it makes
;    impossible for a user program to see the address space of another program that's running. As far as the
;    program is concerned, it's the only program running. 
;
;    With this memory scheme we can map multiple processes at the same address. In real memory they're in 
;    different places.The virtual addresses can be swapped between the processes so one address can point to
;    the different process after swap.
;
;    Aby virtual addresses and physical addresses thtat those virtual addresses point to, they all need to be 
;    divisible by 496.
;
; 7) 4GB of addressable memory:
;    - We gain access to 32 - bit instructions and can easily work with 32 - bit registers,
;    - We can address up to 4GB of memory at any time and we are no longer limited to the 1MB
;      of memory provided by real mode.
;
; 8) Global descriptor table (https://wiki.osdev.org/Global_Descriptor_Table):
;    - it is specific to the IA32 architecture,
;    - it contains entries telling the CPU about memory segments,
;    - the GDT is loaded using the LGDT assembly instruction,
;    - GDT is a pointer to the actual descriptor entries themsleves,
;    - GDT entry describe the memory and the access rights to that memory,
;
; A20 Line (https://wiki.osdev.org/A20_Line)
; 
; 1) Fast A20 Gate:
;    On most newer computers starting with the IBM PS/2, the chipset has a FAST A20
;    option that can quickly enable the A20 line. To enable A20 this way, there is no need
;    for delay loops or polling, just 3 simple instructions.
;
;    in al, 0x92    in - reads from the processor bus
;    or al, 2
;    out 0x92, al   out - writes to processor bus
;
;
; Alignment issues
;
; C requires functions to be aligned in a certain way and when we're writing assembly in kernel.asm for example, 
; and we're linking with object files created with the C compiler, our assembly might not align correctly. This 
; means that all those functions compiled wont be aligned (compiler thinks they are aligned) because of our assembly
; code when its loaded into memory. The reason this is happening is because our assembly shares the same section as
; our object files created with the C compiler.
;
; We need our assembly aligned to 16 Bytes because thats what the C compiler is using by default:
; 
; 512 % 16 = 0 -> if we align our kernel.asm file to 512 bytes, then it ill be aligned by default
;
; We also need to create another section for .asm files at the very end of the linker script. This means that
; our assembly will never affect other sections alignment. The only problem of that solution is that we cannot
; use our assembly section in kernel.asm becuase our output format is binary (set in linker). That;s why 
; the kernel.asm file has to be always the first one to be linked (the first file in FILES section of Makefile)
;
; Becuase of the above, our code will first jump to 0x100000 and so on and not to some C code. The idea is
; that at the end of kernel.asm we will jump to some C function. 
;
; We cannot put our kernel.asm in he back of the memory so we need to align the asm data in different way so it 
; wont damge the alignment. That is why, at the bottom of kernel.asm we put the instruction:
;
; times 512-($ - $$) db 0
;
; whenever we want our assembly file in the last section in the linker we can put the instruction:
; section .asm -> .asm is the name of our section

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