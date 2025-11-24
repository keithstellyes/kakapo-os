all: kakapo.bin

CC :=  i686-elf-gcc -ffreestanding -fno-rtti -fno-exceptions -std=c++23 -O2 -Wall -Wextra -DQEMU -DKAKAPO_OS

cppruntime.o: src/kernel/cppruntime.cpp
	$(CC) -c src/kernel/cppruntime.cpp -std=c++23

boot.o: src/kernel/boot.s
	i686-elf-as src/kernel/boot.s -o boot.o

kernel.o: src/kernel/kernel.cpp src/kernel/vga.hpp src/kernel/keyboard.hpp src/kernel/io.h \
	src/kernel/power.h src/kernel/string.h src/kernel/KakapoShell.hpp src/kernel/rsdp.h

	$(CC) -c src/kernel/kernel.cpp -o kernel.o

kakapo.bin: cppruntime.o boot.o kernel.o linker.ld
	$(CC) -T linker.ld -o kakapo.bin cppruntime.o boot.o kernel.o -lgcc -nostdlib

.PHONY: clean

clean:
	rm -f boot.o kernel.o cppruntime.o kakapo.bin
