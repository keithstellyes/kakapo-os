#include <cstdlib>
#include <cstring>
#include <iostream>
#include "realloc.h"
/*
 * A sandbox to make arbitrary modifications to the custom allocator :)
 */

void doMalloc();
void printRegions();
void doFree();
void doRealloc();
bool invariantsPass();
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
        if(!invariantsPass()) {
            return 1;
        }
        if(std::cin.eof()) {
            break;
        }
        std::cout << "m <number>: number of bytes to allocate\n";
        std::cout << "f <index>: free region at index\n";
        std::cout << "r <index> <number> pointer at region <index> to resize to <number> bytes\n";
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
            case 'f':
                doFree();
                break;
            case 'r':
                doRealloc();
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
    size_t totalRegions = 0;
    memregionheader_t *head = allocator.head;
    int index = 0;
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
        std::cout << " @ [" << index++ << "]" << (void*)(head + 1);
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

memregionheader_t *fromIndex(unsigned int regionIndex)
{
    memregionheader_t *head = allocator.head;
    for(unsigned int i = 0; i < regionIndex; i++) {
        if(head == NULL) {
            std::cout << "There doesn't seem to be that many memory regions...\n";
            return NULL;
        }
        head = nextRegion(head);
    }
    if(head == NULL) {
        std::cout << "There doesn't seem to be that many memory regions...\n";
        return NULL;
    }
    return head;
}

void doFree()
{
    unsigned int regionIndex;
    std::cin >> regionIndex;
    memregionheader_t *head = fromIndex(regionIndex);
    std::cout << kakapoFree(head+1) << '\n';
}

void doRealloc()
{
    unsigned int regionIndex;
    std::cin >> regionIndex;
    memregionheader_t *head = fromIndex(regionIndex);
    size_t new_size;
    std::cin >> new_size;
    std::cout << "Reallocation..." << (kakapoRealloc(head+1, new_size) ? "successful!" : "failed!") << '\n';
}

bool invariantsPass()
{
    bool allPass = true;
    if(!memregionSizeIsConsistent()) {
        std::cerr << "Invariant failed: \033[31mthe tracked sizes of headers plus";
        std::cerr << " the sizes of the headers themselves have exceeded the";
        std::cerr << " total memory they were allocated.\n";
        std::cerr << "\033[0m";
        allPass = false;
    }
    if(!noEmptyMemoryRegions()) {
        std::cerr << "Invariant failed: \033[31mthere shouldn't be any empty memory regions.\n";
        std::cerr << "\033[0m";
        allPass = false;
    }

    return allPass;
}
