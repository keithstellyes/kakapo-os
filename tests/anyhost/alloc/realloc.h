#pragma once

#include "malloc.h"
#include "free.h"
void *kakapoRealloc(void *ptr, size_t new_size)
{
    if(!ptr) {
        return malloc(new_size);
    }
    if(new_size == 0) {
        // realloc(new_size=0) is undefined behavior, we interpret it as a free
        kakapoFree(ptr);
        return NULL;
    }
    memregionheader_t *owningHeader = ((memregionheader_t*)ptr) - 1;
    memregionheader_t *head = allocator.head;
    while(head) {
        if(head == owningHeader) {
            if(!head->isReserved) {
                // not already alloc'd...
                return NULL;
            }
            if(new_size == head->size) {
                // no-op
                return ptr;
            } else if(new_size > head->size) {
                size_t additionalBytesNeeded = new_size - head->size;
                memregionheader_t *next = nextRegion(head);
                // is it possible to steal from our neighbor so we don't have to move?
                if(next && !next->isReserved && next->size + sizeof(memregionheader_t) >= additionalBytesNeeded) {
                    if(next->size - additionalBytesNeeded >= MIN_REGION_SIZE) {
                        // OK to steal from neighbor without needing to consume them
                        head->size += additionalBytesNeeded;

                        // move the next header to compensate for this header growing
                        memregionheader_t tempNext = *next;
                        tempNext.size -= additionalBytesNeeded;
                        // next pointer's information is invalidated
                        next = nextRegion(head);
                        *next = tempNext;
                        return head + 1;
                    } else {
                        // Ok, we cannot steal from neighbor so we consume them.

                        if(!memregionTryMerge(head, next)) {
                            // shouldn't happen at this point?
                            // assert(false)
                            return NULL;
                        }
                        return ptr;
                    }
                } else {
                    void *newPtr = malloc(new_size);
                    if(!newPtr) {
                        return NULL;
                    }
                    memcpy(newPtr, ptr, owningHeader->size);
                    kakapoFree(ptr);
                    return newPtr;
                }
            } else {
                void *newPtr = malloc(new_size);
                if(!newPtr) {
                    return NULL;
                }
                memcpy(newPtr, ptr, owningHeader->size);
                kakapoFree(ptr);
                return newPtr;
            }
        }
        head = nextRegion(head);
    }
    return NULL;
}

#ifdef DEFINE_REALLOC
void *realloc( void *ptr, size_t new_size )
{
    return kakapoRealloc(ptr, new_size);
}
#endif
