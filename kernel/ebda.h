#pragma once
inline unsigned char *get_ebda_address()
{
    void *ptr = (void*)0x40E;
    uint32_t kbOffset = *((uint16_t *)(ptr));
    uint32_t address = kbOffset << 4;
    return (unsigned char*)address;
}
