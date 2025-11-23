#include <iostream>

#include "elf.hpp"
#include <cstdio>

void printElf(Elf &elf);

int main(int argc, char **argv)
{
    if(argc != 2) {
        std::cerr << "Usage:" << argv[0] << std::endl;
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if(fseek(f, 0L, SEEK_END)) {
        perror("Failed to seek to end of file.");
        fclose(f);
        return 2;
    }
    size_t sz = ftell(f);
    rewind(f);
    unsigned char *buffer = (unsigned char*)malloc(sz);
    if(!buffer) {
        std::cerr << "Failed to allocate buffer to read in ELF file" << std::endl;
        fclose(f);
        return 3;
    }
    size_t total_read = fread(buffer, 1, sz, f);
    fclose(f);
    if(total_read != sz) {
        std::cerr << "Failed to read all expected bytes in file" << std::endl;
        return 4;
    }
    Elf elf(buffer, sz);
    free(buffer);

    std::cout << (uint16_t)elf.type << '\n';
    if(!elf.isValid) {
        std::cerr << "Not a valid ELF file." << std::endl;
        return 5;
    }
    printElf(elf);
    return 0;
}

void printElf(Elf &elf)
{
    std::cout << "Endianness:" << (elf.endianness == endian::little ? "little" : "big") << '\n';
    std::cout << (elf.bits == ElfBits::_32 ? "32" : "64") << " bits\n";
    std::cout << ElfTypeToString(elf.type) << '\n';
}

