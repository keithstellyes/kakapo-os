#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stddef.h>

typedef struct memregionheader_t {
    // the number of bytes that are in this region not including space taken by the header
    size_t size;
    bool isReserved;
} memregionheader_t;

typedef struct allocator_t {
    memregionheader_t *head;
    // redundant variables defined for convenience
    // size is the number of bytes in this contiguous region available for use by anything
    size_t size;
    void *endOfMemory;
} allocator_t;

static allocator_t allocator;

memregionheader_t *createRegion(void *data, size_t size)
{
    if(size < sizeof(memregionheader_t)) {
        // literally impossible
        return NULL;
    }

    memregionheader_t *newRegion = (memregionheader_t*)data;
    newRegion->size = size - sizeof(memregionheader_t);
    newRegion->isReserved = false;
    return newRegion;
}

void initAllocator(void *memoryStart, size_t size)
{
    allocator.head = createRegion(memoryStart, size);
    allocator.head->isReserved = false;
    // note that this is the total number of bytes, but does not track how many bytes are in use by headers
    allocator.size = size;
    allocator.endOfMemory = ((unsigned char*)memoryStart) + size;
}

memregionheader_t *nextRegion(memregionheader_t *head)
{
    if(!head || ((void*)head) >= allocator.endOfMemory) {
        return NULL;
    }
    memregionheader_t *next = (memregionheader_t*)(((unsigned char*)head) + head->size + sizeof(memregionheader_t));
    if((void*)next >= allocator.endOfMemory) {
        return NULL;
    }
    return next;
}

/*
 * Tracks the invariant that we're not accidentally "gaining" or "losing" memory
 * Returns true if the invariant is not violated
 *
 * Undefined behavior if the allocator has not yet been init
 */
bool memregionSizeIsConsistent()
{
    memregionheader_t *head = allocator.head;
    size_t total = 0;
    size_t numberOfRegions = 0;
    while(head) {
        numberOfRegions++;
        total += head->size;
        if(total >= allocator.size) {
            return false;
        }
        head = nextRegion(head);
    }

    return total + numberOfRegions * sizeof(memregionheader_t) == allocator.size;
}

bool noEmptyMemoryRegions()
{
    memregionheader_t *head = allocator.head;
    while(head) {
        if(head->size == 0) {
            return false;
        }
        head = nextRegion(head);
    }
    return true;
}
#ifdef __cplusplus
}
#endif
