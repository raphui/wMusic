#include "streamManager.h"

int loadStreamer( const char *url )
{
    TRACE_2( STREAMMANAGER , "loadStream( %s )." , url );

    int status = PC_SUCCESS;


    return status;
}

int playStreamer( const char *addr )
{
    TRACE_2( STREAMMANAGER , "playStream( %s )." , addr );

    int status = PC_SUCCESS;


    return status;
}

int pauseStreamer( const char *addr )
{
    TRACE_2( STREAMMANAGER , "pauseStream( %s )." , addr );

    int status = PC_SUCCESS;


    return status;
}

int getMulticastAddr( const char *url )
{
    TRACE_2( STREAMMANAGER , "getMulticastAddr( %s )." , url );

    int status = PC_SUCCESS;
    int ret = 0;

    char *addr = ( char * )zmalloc( 14 * sizeof( char ) );

    memset( addr , 0 , 14 );

    TRACE_3( STREAMMANAGER , "Getting the address.");

    ret = getNextMulticastAddr( addr );

    if( ret == PC_ERROR )
    {
        TRACE_ERROR( STREAMMANAGER , "Fail to get a multicast address.");

        status = PC_ERROR;
    }
    else
    {
        TRACE_3( STREAMMANAGER , "Retrieve the address : %s" , addr );

        sendVoid( addr , 14 );
    }

    return status;
}
