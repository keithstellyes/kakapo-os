#pragma once
#ifdef __cplusplus
extern "C" {
#endif

memregionheader_t *memregionTryMerge(memregionheader_t *a, memregionheader_t *b)
{
    if(!a || !b || a->isReserved || b->isReserved) {
        return NULL;
    }
    memregionheader_t *consumer = a < b ? a : b;
    memregionheader_t *victim = b > a ? b : a;

    memregionheader_t *regionFollowingConsumer = (memregionheader_t*)(((unsigned char*)consumer) + consumer->size + sizeof(memregionheader_t));
    if(victim != regionFollowingConsumer) {
        // they have to be adjacent to merge
        return NULL;
    }
    consumer->size += victim->size + sizeof(memregionheader_t);
    return consumer;
}

const char *kakapoFree(void *ptr)
{
    if(ptr == NULL) {
        return "Given ptr was NULL"; /* no-op */
    }

    memregionheader_t *prev = NULL;
    memregionheader_t *owningHeader = ((memregionheader_t*)ptr) - 1;
    memregionheader_t *head = allocator.head;
    while(head) {
        if(head == owningHeader) {
            if(!head->isReserved) {
                return "Double free!";
            }
            head->isReserved = false;
            memregionheader_t *prevPlusHead = memregionTryMerge(prev, head);
            head = prevPlusHead ? prevPlusHead : head;
            memregionheader_t *next = nextRegion(head);
            memregionheader_t *headPlusNext = memregionTryMerge(head, next);
            if(prevPlusHead && headPlusNext) {
                return "Free successful; two merges";
            } else if(prevPlusHead) {
                return "Free successful; previous merged with head";
            } else if(headPlusNext) {
                return "Free successful; head merged with next";
            }
            return "Free successful; no merges";
        }
        prev = head;
        head = nextRegion(head);
    }

    return "Failed to free!";
}

#ifdef DEFINE_FREE
void free(void *ptr)
{
    kakapoFree(ptr);
}
#endif

#ifdef __cplusplus
}
#endif

