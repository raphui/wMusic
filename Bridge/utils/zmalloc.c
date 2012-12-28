#include "zmalloc.h"

static int memoryAllocate = 0;

void *zmalloc( size_t size )
{
    TRACE_2( ZMALLOC , "zmalloc( %d )." , size );

    memoryAllocate += size;

    return malloc( size );
}

int getMemoryCount( void )
{
    TRACE_2( ZMALLOC , "getMemoryCount().");

    return memoryAllocate;
}
