#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "vga.hpp"
#include "keyboard.hpp"
#include "halt.h"
#include "power.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#define VGA_MEMORY  0xB8000

char SHIFT_TABLE[128];

void init_shift_table()
{
    for(int i = 0; i < 128; i++) {
        SHIFT_TABLE[i] = i;
    }
    const char keys[] = "`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./";
    const char vals[] = "~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?";
    static_assert(sizeof(keys) == sizeof(vals), "keys and vals should be same length.");
    for(unsigned int i = 0; i < sizeof(keys) - 1; i++) {
        SHIFT_TABLE[(int)keys[i]] = vals[i];
    }
}

extern "C" void kernel_main(void)
{
    init_shift_table();
    VgaTerminal qemuvga((uint16_t*) VGA_MEMORY);
    const char greeting[] = "Hello, World from KakapoOS!\nAnd also from here!\nI need to get the bars over the letters working :)";
    qemuvga.writeString(greeting);
    qemuvga.update_cursor();

    bool shiftDown = false;
    while(1) {
        keyboard_input in = next_input();
        if(in.is_ascii) {
            if(in.ascii == 0x1B) {
                shutdown();
            }
            char ascii = shiftDown ? SHIFT_TABLE[(int)in.ascii] : in.ascii;
            qemuvga.putchar(ascii);
            qemuvga.update_cursor();
        } else {
            if(in.isShift()) {
                if(in.pressed == PRESSED) {
                    shiftDown = true;
                } else {
                    shiftDown = false;
                }
            } else {
                switch(in.special_value) {
                    case keyboard_special_type::CURSOR_UP:
                        qemuvga.updateCursorRow(qemuvga.row - 1);
                        break;
                    case keyboard_special_type::CURSOR_DOWN:
                        qemuvga.updateCursorRow(qemuvga.row + 1);
                        break;
                    case keyboard_special_type::CURSOR_LEFT:
                        qemuvga.updateCursorColumn(qemuvga.column - 1);
                        break;
                    case keyboard_special_type::CURSOR_RIGHT:
                        qemuvga.updateCursorColumn(qemuvga.column + 1);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

