#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "vga.hpp"
#include "keyboard.hpp"
#include "halt.h"
#include "power.h"
#include "KakapoShell.hpp"
#include "rsdp.h"
#include "sdt.h"
#include "multiboot.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#define VGA_MEMORY  0xB8000

void printSdt(rsdp_t *rsdp, Terminal &terminal)
{
    SDT *rsdt = (SDT*)rsdp->RsdtAddress;
    int entries = (rsdt->h.Length - sizeof(rsdt->h)) / 4;

    for (int i = 0; i < entries; i++)
    {
        ACPISDTHeader *h = (ACPISDTHeader *) rsdt->next[i];
        char buff[5];
        buff[0] = h->Signature[0];
        buff[1] = h->Signature[1];
        buff[2] = h->Signature[2];
        buff[3] = h->Signature[3];
        buff[4] = '\0';
        // terminal.writeString(buff);
        // terminal.writeString(",");
    }
}

extern "C" void kernel_main(multiboot_info_t *mbd, unsigned int magic)
{
    VgaTerminal qemuvga((uint16_t*) VGA_MEMORY);
    init_shift_table();
    rsdp_t *rsdp = NULL;
    /* Make sure the magic number matches for memory mapping*/
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        qemuvga.writeStringLine("invalid magic number!");
    }

    /* Check bit 6 to see if we have a valid memory map */
    if(!(mbd->flags >> 6 & 0x1)) {
        qemuvga.writeStringLine("invalid memory map given by GRUB bootloader");
    }

    /* Loop through the memory map and display the values */
    int i;

    for(i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt =
            (multiboot_memory_map_t*) (mbd->mmap_addr + i);

        //printf("Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
        //        mmmt->addr, mmmt->len, mmmt->size, mmmt->type);

        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE || mmmt->type == MULTIBOOT_MEMORY_ACPI_RECLAIMABLE) {
            char numbuff[12];
            // mmmt->size is the size of the structure itself, from the spec:
            // where 'size' is the size of the associated structure in bytes, which can be greater than the minimum of 20 bytes.
            size_t kilobytes = mmmt->len / 1000;
            if(kilobytes < 1000) {
                continue;
            }
            itoa_decimal(kilobytes, numbuff);
            qemuvga.writeString("Memory: ");
            qemuvga.writeString(numbuff);
            qemuvga.writeString("K (");
            itoa_decimal(kilobytes / 1000, numbuff);
            qemuvga.writeString(numbuff);
            qemuvga.writeString("M)\n");
        }
    }

    find_rsdp(&rsdp);
    if(rsdp) {
        // qemuvga.writeString("BIOS OEM:");
        // qemuvga.writeStringLine(rsdp->OEMID);
        if(rsdp->Revision == 0) {
            // qemuvga.writeString("ACPI 1.0 detected...");
        } else if(rsdp->Revision == 2) {
            // qemuvga.writeString("ACPI 2.0 or later detected...");
        } else {
            qemuvga.writeStringLine("Unknown ACPI version");
        }
        printSdt(rsdp, qemuvga);
    }
    qemuvga.writeStringLine("\nWelcome to KakapoOS!");
    qemuvga.update_cursor();
    KakapoShell shell(&qemuvga);

    while(!shell.halting) {
        keyboard_input in = next_input();
        shell.onInput(in);
    }
    qemuvga.clearScreen();
    qemuvga.updateCursorRow(0);
    qemuvga.updateCursorColumn(0);
    qemuvga.writeString("Bye");
    halt();
}

