#include "io.h"

void shutdown()
{
    // QEMU specific method: https://wiki.osdev.org/Shutdown
#ifdef QEMU
    outw(0x604, 0x2000);
#endif
}
