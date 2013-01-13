#include "streamManager.h"

int initStreamer( void )
{
    TRACE_2( STREAMMANAGER , "initStreamer().");

    int status = PC_SUCCESS;
    int i;

    for( i = 0 ; i < MAX_STREAM ; i++ )
    {
        streamProps[i].url = ( char * )zmalloc( 25 * sizeof( char ) );
        streamProps[i].name = ( char * )zmalloc( 255 * sizeof( char ) );

        strcpy( streamProps[i].url , "");
        strcpy( streamProps[i].name , "");
    }

    return status;
}

int loadStreamer( const char *url )
{
    TRACE_2( STREAMMANAGER , "loadStream( %s )." , url );

    int status = PC_SUCCESS;


    return status;
}

int playStreamer( const char *name )
{
    TRACE_2( STREAMMANAGER , "playStream( %s )." , name );

    return playStream( name );
}

int pauseStreamer( const char *name )
{
    TRACE_2( STREAMMANAGER , "pauseStream( %s )." , name );

    return pauseStream( name );
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
        TRACE_1( STREAMMANAGER , "Retrieve the address : %s" , addr );

        sendVoid( addr , 14 );
    }

    return status;
}

int registerNewStream( char *url , char *name )
{
    TRACE_2( STREAMMANAGER , "registerNewStream( %s , %s )." , url , name );

    int status;
    int i;

    for( i = 0 ; i < MAX_STREAM ; i++ )
    {
        if( strcmp( streamProps[i].name , "" ) == 0 )
        {
            TRACE_3( STREAMMANAGER , "Settings the properties.");

            memcpy( streamProps[i].url , url , strlen( url ) );
            memcpy( streamProps[i].name , name , strlen( name ) );

            status = PC_SUCCESS;

            break;
        }
        else
        {
            TRACE_3( STREAMMANAGER , "Index: %d , Url: %s , Name: %s" , i , url , name );

            status = PC_ERROR;
        }
    }

    if( status == PC_ERROR )
        TRACE_ERROR( STREAMMANAGER , "Cannot assign a new stream, the max is reached.");

    return status;
}

int unregisterStream( char *name )
{
    TRACE_2( STREAMMANAGER , "unregisterStream( %s )." , name );

    int status = PC_SUCCESS;
    int i;

    for( i = 0 ; i < MAX_STREAM ; i++ )
    {
        if( strcmp( streamProps[i].name , name ) == 0 )
        {
            strcpy( streamProps[i].url , "");
            strcpy( streamProps[i].name , "");
        }
        else
        {
            TRACE_ERROR( STREAMMANAGER , "Stream: %s has been found !" , name );

            status = PC_ERROR;
        }
    }

    return status;
}
