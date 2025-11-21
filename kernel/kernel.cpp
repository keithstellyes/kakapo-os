#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "vga.hpp"
#include "keyboard.hpp"
#include "halt.h"
#include "power.h"
#include "KakapoShell.hpp"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#define VGA_MEMORY  0xB8000


extern "C" void kernel_main(void)
{
    init_shift_table();
    VgaTerminal qemuvga((uint16_t*) VGA_MEMORY);
    const char greeting[] = "Hello, World from KakapoOS!\nAnd also from here!\nI need to get the bars over the letters working :)";
    qemuvga.writeString(greeting);
    qemuvga.update_cursor();
    KakapoShell shell(&qemuvga);

    while(1) {
        keyboard_input in = next_input();
        shell.onInput(in);
    }
}

