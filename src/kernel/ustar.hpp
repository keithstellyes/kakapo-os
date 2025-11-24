#pragma once

#include <stdint.h>

enum class UstarTypeFlag {
    // normal can be one of two possible values (:
    NormalAsAscii = '0',
    NormalAsNul = '\0',
    HardLink = '1',
    SymbolicLink = '2',
    CharacterDevice = '3',
    BlockDevice = '4',
    Directory = '5',
    Fifo = '6'
};

struct ustarheader_t {
    unsigned char filename[100];
    uint64_t filemode;
    uint64_t ownerId;
    uint64_t groupId;
    unsigned char filesizeInOctal[12];
    unsigned char modificationTimeInOctal[12];
    uint64_t checksum;
    UstarTypeFlag type:8;
    unsigned char linkedFilename[100];
    unsigned char magic[6];
    unsigned char version[2];
    unsigned char username[32];
    unsigned char groupname[32];
    uint64_t deviceMajorNumber;
    uint64_t deviceMinorNumber;
    unsigned char filenamePrefix[155];
    unsigned char _padding[12];
} __attribute__((packed));

static_assert(sizeof(ustarheader_t) == 512);

int oct2bin(unsigned char *str, int size) {
    int n = 0;
    unsigned char *c = str;
    while (size-- > 0) {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}

bool ustarHeaderIsValid(ustarheader_t *header)
{
    if(strncmp(header->magic, "ustar") != 0) {
        return false;
    }
}
