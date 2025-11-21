all: kakapo.bin

CC :=  i686-elf-gcc -ffreestanding -fno-rtti -fno-exceptions -std=c++23 -O2 -Wall -Wextra -DQEMU -DKAKAPO_OS

cppruntime.o: kernel/cppruntime.cpp
	$(CC) -c kernel/cppruntime.cpp -std=c++23

boot.o: kernel/boot.s
	i686-elf-as kernel/boot.s -o boot.o

kernel.o: kernel/kernel.cpp kernel/vga.hpp kernel/keyboard.hpp kernel/io.h kernel/power.h kernel/string.h kernel/KakapoShell.hpp
	$(CC) -c kernel/kernel.cpp -o kernel.o

kakapo.bin: cppruntime.o boot.o kernel.o linker.ld
	$(CC) -T linker.ld -o kakapo.bin cppruntime.o boot.o kernel.o -lgcc -nostdlib

.PHONY: clean

clean:
	rm -f boot.o kernel.o cppruntime.o kakapo.bin
