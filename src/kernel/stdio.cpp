#include "File.hpp"
#include "stdio.h"

struct FILE {
    File *f;
};

extern "C" size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream )
{
    return stream->f->write(ptr, size, count);
}

extern "C" size_t fread ( void * ptr, size_t size, size_t count, FILE * stream )
{
    return stream->f->read(ptr, size, count);
}

