#pragma once

void halt()
{
    __asm__ volatile ("hlt");
}
