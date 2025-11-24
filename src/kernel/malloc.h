#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "alloc.h"

#ifndef MIN_REGION_SIZE
#define MIN_REGION_SIZE 1000
#endif
// inline to ensure a simple passthru by the function called simply 'malloc'
inline void *kakapoMalloc(size_t requestedSize)
{
    if(requestedSize + sizeof(memregionheader_t) >= allocator.size) {
        // impossible!
        return NULL;
    }
    if(requestedSize == 0) {
        // supposedly it's up to implementations to define behavior here
        // We decide NULL so it's one less sitatuion we have to worry about
        return NULL;
    }

    memregionheader_t *head = allocator.head;
    while(head) {
        if(head->isReserved) {
            head = nextRegion(head);
            continue;
        } else if(requestedSize > head->size) {
            head = nextRegion(head);
            continue;
        }

        // recall that the memory a header tracks _immediately_ follows
        void* result = head + 1;
        // can we not only split, but the new region created will actually have
        // a decent change of fulfilling a request?
        bool shouldSplit = requestedSize < sizeof(memregionheader_t) + MIN_REGION_SIZE;
        head->isReserved = true;
        if(shouldSplit) {
            memregionheader_t *newRegion = createRegion((unsigned char*)result + requestedSize, head->size - requestedSize);
            // assert(head->size + sizeof(memregionheader_t) + newRegion->size == head->size);
            head->size = requestedSize;
        }
        return result;
    }

    return NULL;
}

#ifdef DEFINE_MALLOC
void *malloc(size_t size)
{
    return kakapoMalloc(size);
}
#endif

#ifdef __cplusplus
}
#endif
