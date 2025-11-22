#pragma once
#include <stddef.h>

typedef struct FILE FILE;

static FILE *stdin;
static FILE *stdout;
size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );

