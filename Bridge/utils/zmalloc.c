#include "zmalloc.h"

static int memoryAllocate = 0;

void *zmalloc( size_t size )
{
    memoryAllocate += size;

    return malloc( size );
}


