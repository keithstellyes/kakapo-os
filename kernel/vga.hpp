#ifndef MYOS_VGA_DEV_H
#define MYOS_VGA_DEV_H
#include "io.h"
#include "Terminal.hpp"
#include "string.h"

#define VGA_WIDTH   80
#define VGA_HEIGHT  25

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};


static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}
class VgaTerminal : public Terminal {
    public:
        uint8_t row, column, color;
        uint16_t *buffer;

        VgaTerminal(uint16_t *buffer)
        {
            this->buffer = buffer;
            row = 0;
            column = 0;
            color = vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
            for (size_t y = 0; y < VGA_HEIGHT; y++) {
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                    const size_t index = y * VGA_WIDTH + x;
                    buffer[index] = vga_entry(' ', color);
                }
            }

        }
        void putentryat(char c, uint8_t color, size_t x, size_t y)
        {
            if(x >= VGA_WIDTH || y >= VGA_HEIGHT) {
                return;
            }
            const size_t index = y * VGA_WIDTH + x;
            buffer[index] = vga_entry(c, color);
        }

        void putchar(char c)
        {
            if(column >= VGA_WIDTH) {
                column = VGA_WIDTH - 1;
            }

            if(row >= VGA_HEIGHT) {
                row = VGA_HEIGHT - 1;
            }
            if(c == '\n') {
                column = 0;
                row++;
                return;
            } else if(c == '\b') {
                if(column <= 0) {
                    column = VGA_WIDTH - 1;
                    row--;
                } else {
                    column--;
                    putentryat(' ', color, column, row);
                }
                return;
            }
            putentryat(c, color, column, row);
            if (++column == VGA_WIDTH) {
                column = 0;
                if (++row == VGA_HEIGHT)
                    row = 0;
            }
        }
        void write(const char* data, size_t size)
        {
            for (size_t i = 0; i < size; i++)
                putchar(data[i]);
        }
        void writeString(const char *string)
        {
            write(string, strlen(string));
        }
        void updateCursorRow(unsigned int row) {
            this->row = row;
            update_cursor(column, row);
        }
        void updateCursorColumn(unsigned int column) {
            this->column = column;
            update_cursor(column, row);
        }
        void update_cursor()
        {
            update_cursor(column, row);
        }

        void update_cursor(int x, int y)
        {
            if(x >= VGA_WIDTH || y >= VGA_HEIGHT) {
                update_cursor(0, 0);
                return;
            }
            uint16_t pos = y * VGA_WIDTH + x;

            outb(0x3D4, 0x0F);
            outb(0x3D5, (uint8_t) (pos & 0xFF));
            outb(0x3D4, 0x0E);
            outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
        }
};
#endif // MYOS_VGA_DEV_H
