#include "zmemory.h"

static int memoryAllocate = 0;

void *zmalloc( size_t size )
{
    TRACE_2( ZMEMORY , "zmalloc( %d )." , size );

    memoryAllocate += size;

    return malloc( size );
}

int getMemoryCount( void )
{
    TRACE_2( ZMEMORY , "getMemoryCount().");

    return memoryAllocate;
}
