#include "streamManager.h"

void setDispatcherIp( char *ip )
{
    TRACE_2( STREAMMANAGER , "setDispatcherIp( %s )." , ip );

    size_t size = strlen( ip );

    dispatcherIp = ( const char * )zmalloc( size * sizeof( char ) );

    //snprintf( dispatcherIp , size , "%s" , ip );

    memcpy( dispatcherIp , ip , size );
}

int playStreamer( char *url )
{
    TRACE_2( STREAMMANAGER , "playStreamer( %s )." , url );

    int status = PC_SUCCESS;
    int ret;

    ret = playUrl( url );

    if( ret == PC_ERROR )
    {
        TRACE_ERROR( STREAMMANAGER , "Fail to play the url : %s" , url );

        status = PC_ERROR;
    }

    return status;
}
