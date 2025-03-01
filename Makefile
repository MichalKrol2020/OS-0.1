FILES = ./build/kernel.asm.o ./build/kernel.o ./build/stdio.o ./build/idt/idt.asm.o ./build/idt/idt.o ./build/memory/memory.o ./build/io/io.asm.o ./build/memory/heap/heap.o ./build/memory/heap/kheap.o ./build/memory/paging/paging.o ./build/memory/paging/paging.asm.o ./build/graphics/graphics.o ./build/graphics/graphics-mem.o ./build/startup/logo/logo-data.o ./build/startup/startup.o ./build/timer/timer.o ./build/collections/linked-list.o ./build/animations/animations.o ./build/memory/memory.asm.o # whenever the all command is run, we depend on the files and it will go through the files and resolve the labels
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

./build/memory/memory.asm.o: ./src/memory/memory.asm
	nasm -f elf -g ./src/memory/memory.asm -o ./build/memory/memory.asm.o

./build/memory/memory.o: ./src/memory/memory.c
	i686-elf-gcc $(INCLUDES) -I./src/memory $(FLAGS) -std=gnu99 -c ./src/memory/memory.c -o ./build/memory/memory.o

./build/io/io.asm.o: ./src/io/io.asm
	nasm -f elf -g ./src/io/io.asm -o ./build/io/io.asm.o

./build/memory/heap/heap.o: ./src/memory/heap/heap.c
	i686-elf-gcc $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/heap.c -o ./build/memory/heap/heap.o

./build/memory/heap/kheap.o: ./src/memory/heap/kheap.c
	i686-elf-gcc $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/kheap.c -o ./build/memory/heap/kheap.o

./build/memory/paging/paging.o: ./src/memory/paging/paging.c
	i686-elf-gcc $(INCLUDES) -I./src/memory/paging $(FLAGS) -std=gnu99 -c ./src/memory/paging/paging.c -o ./build/memory/paging/paging.o

./build/memory/paging/paging.asm.o: ./src/memory/paging/paging.asm
	nasm -f elf -g ./src/memory/paging/paging.asm -o ./build/memory/paging/paging.asm.o

./build/graphics/graphics.o: ./src/graphics/graphics.c
	i686-elf-gcc $(INCLUDES) -I./src/graphics $(FLAGS) -std=gnu99 -c ./src/graphics/graphics.c -o ./build/graphics/graphics.o

./build/graphics/graphics-mem.o: ./src/graphics/graphics-mem.c
	i686-elf-gcc $(INCLUDES) -I./src/graphics $(FLAGS) -std=gnu99 -c ./src/graphics/graphics-mem.c -o ./build/graphics/graphics-mem.o

./build/startup/startup.o: ./src/startup/startup.c
	i686-elf-gcc $(INCLUDES) -I./src/startup $(FLAGS) -std=gnu99 -c ./src/startup/startup.c -o ./build/startup/startup.o

./build/startup/logo/logo-data.o: ./src/startup/logo/logo-data.c
	i686-elf-gcc $(INCLUDES) -I./src/startup/logo $(FLAGS) -std=gnu99 -c ./src/startup/logo/logo-data.c -o ./build/startup/logo/logo-data.o

./build/timer/timer.o: ./src/timer/timer.c
	i686-elf-gcc $(INCLUDES) -I./src/timer $(FLAGS) -std=gnu99 -c ./src/timer/timer.c -o ./build/timer/timer.o

./build/collections/linked-list.o: ./src/collections/linked-list.c
	i686-elf-gcc $(INCLUDES) -I./src/collections $(FLAGS) -std=gnu99 -c ./src/collections/linked-list.c -o ./build/collections/linked-list.o

./build/animations/animations.o: ./src/animations/animations.c
	i686-elf-gcc $(INCLUDES) -I./src/animations $(FLAGS) -std=gnu99 -c ./src/animations/animations.c -o ./build/animations/animations.o

clean:
	rm -rf ./bin/boot.bin
	rm -rf ./bin/kernel.bin
	rm -rf ./bin/os.bin
	rm -rf $(FILES)
	rm -rf ./build/kernelfull.o
