#pragma once

#ifdef __cplusplus
extern "C" {
#endif

inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0"
                      : "=a"(ret)
                      : "Nd"(port));
    return ret;
}

inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1"
                      :
                      : "a"(val), "Nd"(port));
}

inline void outw(unsigned short port, unsigned short val) {
    __asm__ volatile ("outw %0, %1"
                      :
                      : "a"(val), "Nd"(port));
}

#ifdef __cplusplus
}
#endif
