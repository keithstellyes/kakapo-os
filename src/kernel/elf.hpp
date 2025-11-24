#pragma once
#include <stddef.h>
// GCC implementation
//
enum class endian
{
    /*
    little = __ORDER_LITTLE_ENDIAN__,
    big    = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
    */
    little = 0,
    big = 1,
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    native = 0
#elif
    native = 1
#endif
};

enum class ElfBits
{
    _32 = 1,
    _64 = 2
};

enum class ElfEndianness
{
    little = 1,
    big = 2
};

enum class ElfAbi
{
    SystemV = 0x00,
    Linux = 0x03
};

enum class ElfType {
    Relocatable = 0x01,
    Executable = 0x02,
    SharedObject = 0x03,
    Core = 0x04
};

const char *ElfTypeToString(ElfType t)
{
    switch(t) {
        case ElfType::Relocatable:
            return "Relocatable";
        case ElfType::Executable:
            return "Executable";
        // NOTE: it seems that compiled Linux binaries are often this
        case ElfType::SharedObject:
            return "Shared Object";
        case ElfType::Core:
            return "Core";
    }
    return "?";
}

typedef struct elf_header {
    uint8_t magic[4];
    ElfBits bits:8;
    ElfEndianness endianness:8;
    uint8_t version;
    ElfAbi abi:8;
    uint8_t abi_version;
    uint8_t padding[7];
    ElfType type:16;
} __attribute__((packed)) elf_header;

class Elf {
    public:
        bool isValid:1 = true;
        endian endianness:1;
        ElfBits bits:2;
        ElfType type:3;
        constexpr Elf(unsigned char *data, size_t length)
        {
            if(!hasElfMagic(data, length) || length < sizeof(elf_header)) {
                isValid = false;
                return;
            }
            elf_header *header = (elf_header*)data;
            switch(header->bits) {
                case ElfBits::_32:
                    bits = ElfBits::_32;
                    break;
                case ElfBits::_64:
                    bits = ElfBits::_64;
                    break;
                default:
                    isValid = false;
                    return;
            }
            switch(header->endianness) {
                case ElfEndianness::little:
                    endianness = endian::little;
                    break;
                case ElfEndianness::big:
                    endianness = endian::big;
                    break;
                default:
                    isValid = false;
                    return;
            }
            if(header->version != 1) {
                isValid = false;
                return;
            }
            switch(header->abi) {
                case ElfAbi::SystemV:
                    break;
                default:
                    // unsupported ELF ABI
                    isValid = false;
                    return;
            }
            switch(header->type) {
                case ElfType::Relocatable:
                    type = header->type;
                    break;
                case ElfType::Executable:
                    type = header->type;
                    break;
                case ElfType::SharedObject:
                    type = header->type;
                    break;
                case ElfType::Core:
                    type = header->type;
                    break;
                default:
                    type = header->type;
                    isValid = false;
                    return;
            }
        }
        static bool hasElfMagic(unsigned char *data, size_t length)
        {
            return length >= 4 && data[0] == 0x7f && data[1] == 'E'
                && data[2] == 'L' && data[3] == 'F';
        }
};
