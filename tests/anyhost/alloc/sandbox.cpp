#include <cstdlib>
#include <cstring>
#include <iostream>
#include "malloc.h"
/*
 * A sandbox to make arbitrary modifications to the custom allocator :)
 */

void doMalloc();
void printRegions();

int main(int argc, char **argv)
{
    // I thought about having this be a static array to avoid this malloc call,
    // but I decided to malloc it here to better support buffer over/underflow catching
    size_t buffsz = 10'000;
    uint8_t *allocMasterBuffer = (uint8_t*)malloc(buffsz);


    initAllocator(allocMasterBuffer, buffsz);
    //    init_allocation(allocMasterBuffer, sizeof(allocMasterBuffer));
    // basic test. should never be allowed because of memory manager overhead
    //    static_assert(kakapoMalloc(sizeof(allocMasterBuffer) - sizeof(size_t)) == NULL);
    std::cout << "memory region header size:" << sizeof(memregionheader_t) << '\n';

    while(1) {
        printRegions();
        if(std::cin.eof()) {
            break;
        }
        std::cout << "m <number>: number of bytes to allocate\n";
        std::cout << "q: quit\n";
        char in;
        std::cin >> in;
        if(std::cin.eof()) {
            break;
        }

        switch(in) {
            case 'm':
                doMalloc();
                break;
            case 'q':
                goto exit;
            default:
                if(std::cin.eof()) {
                    goto exit;
                }
                std::cerr << "?\n";
        }
    }
exit:
    return 0;
}

void printRegions()
{
    if(!memregionSizeIsConsistent()) {
        std::cerr << "Invariant failed: the tracked sizes of headers plus";
        std::cerr << " the sizes of the headers themselves have exceeded the";
        std::cerr << " total memory they were allocated.\n\n";
    }
    size_t totalRegions = 0;
    memregionheader_t *head = allocator.head;
    while(head) {
        totalRegions++;
        if(head->isReserved) {
            std::cout << "[ \033[33mUSED\033[0m ] ";
        } else {
            std::cout << "[ \033[32mFREE\033[0m ] ";
        }
        std::cout << "\033[0m";
        if(head->size < 1'000) {
            std::cout << ' ';
            if(head->size < 100) {
                std::cout << ' ';
                if(head->size < 10) {
                    std::cout << ' ';
                }
            }
        }
        std::cout << head->size;
        std::cout << " @ " << (void*)head;
        std::cout << '\n';
        head = nextRegion(head);
    }
    std::cout << totalRegions << " regions with headers having \033[4m";
    std::cout << sizeof(memregionheader_t) * totalRegions << " bytes of overhead.\033[0m\n";
}
void doMalloc()
{
    size_t numBytes;
    std::cin >> numBytes;

    std::cout << "malloc(" << numBytes << ")\n";
    void *region = kakapoMalloc(numBytes);
    if(region == NULL) {
        std::cout << "failed to allocate!\n";
    }
}
