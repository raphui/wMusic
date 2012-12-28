#include "zmemory.h"

static int memoryAllocate = 0;

void *zmalloc( size_t size )
{
    TRACE_2( ZMEMORY , "zmalloc( %d )." , size );

    memoryAllocate += size;

    return malloc( size );
}

void zfree( void *ptr )
{
    TRACE_2( ZMEMORY , "zfree().");

    if( ptr == NULL )
        return;

    size_t sizePtr = *( ( size_t * )ptr );

    memoryAllocate -= sizePtr;

    free( ptr );
}

int getMemoryCount( void )
{
    TRACE_2( ZMEMORY , "getMemoryCount().");

    return memoryAllocate;
}
