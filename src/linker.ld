ENTRY(_start) /* Program entry point */
OUTPUT_FORMAT(binary) /* Resolves all our symbols to binary. There will be no pfile, header, no elf header, just a flat binary*/
SECTIONS
{
    . = 1M; /* Kernel will be loaded into 1MB in memory (1024x1024) -> 0x100000 HEX */
    .text : ALIGN(4096) /* this ensures aligmnent with our sections themselves */
    {
        *(.text)
    }
    
    .rodata : ALIGN(4096)
    {
        *(.rodata)
    }

    .data : ALIGN(4096)
    {
        *(.data)
    }

    .bss : ALIGN(4096)
    {
        *(COMMON)
        *(.bss)
    }

    .asm : ALIGN(4096) /* Store assembly code at the end of memory so it wont destroy our data alignment. If we don;t do that .asm files can mess up ou C data alignment */
    {
        *(.asm)
    }
}
