#ifndef CONFIG_H
#define CONFIG_H

#define OS_TOTAL_INTERRUPTS     512
#define KERNEL_CODE_SELECTOR    0x08u
#define KERNEL_DATA_SELECTOR    0x10u

#define OS_HEAP_SIZE_BYTES      104857600 // 100 MB
#define OS_HEAP_BLOCK_SIZE      4096
#define OS_HEAP_TABLE_ADDRESS   0x00007E00 // Conventional memory - usable memory (480.5 KiB)

#endif