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

    void *realptr;
    size_t sizeptr;

    if( ptr == NULL )
        return;

    realptr = ( char * )ptr - sizeof( size_t );
    sizeptr = *( ( size_t * )realptr );

    TRACE_3( ZMEMORY , "Size to be free : %d." , sizeptr );

    memoryAllocate -= sizeptr;

    free( ptr );
}

char *getMemoryCount( void )
{
    TRACE_2( ZMEMORY , "getMemoryCount().");

    char *buff = ( char * )zmalloc( 1024 * sizeof( char ) );

    memset( buff , 0 , 1024 );

    sprintf( buff , "\nTotal memory allocate : %d\n" , memoryAllocate );

    return buff;
}
