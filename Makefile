FILES = ./build/kernel.asm.o ./build/kernel.o ./build/stdio.o ./build/idt/idt.asm.o ./build/idt/idt.o ./build/memory/memory.o ./build/io/io.asm.o ./build/memory/heap/heap.o ./build/memory/heap/kheap.o # whenever the all command is run, we depend on the files and it will go through the files and resolve the labels
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -omit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/boot.bin ./bin/kernel.bin # we don't need $(FILES) in her because we've done it in the kernel section. all depends on the kernel and kernel depends on the FILE | dd if=/dev/zero bs=512 count 10 >> ./bin/os.bin -> we need this to align data in the sector because our kernel doesn't fit in the sector. We write 10 sectors with 0s so that we don't have do this any time we change our code
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin

./bin/kernel.bin: $(FILES) # links all the files together into one object file
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

./build/kernel.o: ./src/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

./build/stdio.o: ./src/stdio.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/stdio.c -o ./build/stdio.o

./build/idt/idt.asm.o: ./src/idt/idt.asm
	nasm -f elf -g ./src/idt/idt.asm -o ./build/idt/idt.asm.o

./build/idt/idt.o: ./src/idt/idt.c
	i686-elf-gcc $(INCLUDES) -I./src/idt $(FLAGS) -std=gnu99 -c ./src/idt/idt.c -o ./build/idt/idt.o

./build/memory/memory.o: ./src/memory/memory.c
	i686-elf-gcc $(INCLUDES) -I./src/memory $(FLAGS) -std=gnu99 -c ./src/memory/memory.c -o ./build/memory/memory.o

./build/io/io.asm.o: ./src/io/io.asm
	nasm -f elf -g ./src/io/io.asm -o ./build/io/io.asm.o

./build/memory/heap/heap.o: ./src/memory/heap/heap.c
	i686-elf-gcc $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/heap.c -o ./build/memory/heap/heap.o

./build/memory/heap/kheap.o: ./src/memory/heap/kheap.c
	i686-elf-gcc $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/kheap.c -o ./build/memory/heap/kheap.o

clean:
	rm -rf ./bin/boot.bin
	rm -rf ./bin/kernel.bin
	rm -rf ./bin/os.bin
	rm -rf $(FILES)
	rm -rf ./build/kernelfull.o
