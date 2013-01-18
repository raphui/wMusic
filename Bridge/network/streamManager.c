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

int loadStreamer( char *url , char *name )
{
    TRACE_2( STREAMMANAGER , "loadStream( %s , %s )." , url , name );

    registerNewStream( url , name );

    if( strstr( url , "spotify") == NULL )
    {
        return loadStreamFromUrl( url , name );
    }
    else
    {
        return loadMusic( g_session , url );
    }

}

int playStreamer( const char *name )
{
    TRACE_2( STREAMMANAGER , "playStream( %s )." , name );

    int status = PC_SUCCESS;

    int index;

    index = getStreamUrlFromName( name );

    if( index == PC_ERROR )
    {
        TRACE_ERROR( STREAMMANAGER , "Fail to retrieve stream.");

        status = PC_ERROR;
    }
    else
    {
        if( strstr( streamProps[index].url , "spotify") == NULL )
        {
            status = playStream( name );
        }
        else
        {
            status = playMusic( g_session , streamProps[index].url );
        }
    }

    return status;
}

int pauseStreamer( const char *name )
{
    int status = PC_SUCCESS;

    int index;

    index = getStreamUrlFromName( name );

    if( index == PC_ERROR )
    {
        TRACE_ERROR( STREAMMANAGER , "Fail to retrieve stream.");

        status = PC_ERROR;
    }
    else
    {
        if( strstr( streamProps[index].url , "spotify") == NULL )
        {
            status = pauseStream( name );
        }
        else
        {
            status = pauseMusic( g_session , streamProps[index].url );
        }
    }

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
    int sizeUrl;
    int sizeName;

    sizeUrl = strlen( url );
    sizeName = strlen( name );

    for( i = 0 ; i < MAX_STREAM ; i++ )
    {
        if( ( streamProps[i].name == NULL ) || ( strcmp( streamProps[i].name , "" ) == 0 ) )
        {
            TRACE_3( STREAMMANAGER , "Settings the properties.");

            streamProps[i].url = ( char * )zmalloc( sizeUrl * sizeof( char ) );
            streamProps[i].name = ( char * )zmalloc( sizeName * sizeof( char ) );

            memset( streamProps[i].url , 0 , sizeUrl );
            memset( streamProps[i].name , 0 , sizeName );

            sprintf( streamProps[i].url , "%s" , url );
            sprintf( streamProps[i].name , "%s" , name );

            status = PC_SUCCESS;

            TRACE_1( STREAMMANAGER , "Stream : %s , %s is now registered." , url , name );

            TRACE_3( STREAMMANAGER , "Index : %d , Url : %s , Name : %s" , i , streamProps[i].url , streamProps[i].name );

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
        if( ( streamProps[i].name == NULL ) || ( strcmp( streamProps[i].name , name ) == 0 ) )
        {
            strcpy( streamProps[i].url , "");
            strcpy( streamProps[i].name , "");

            zfree( streamProps[i].url );
            zfree( streamProps[i].name );

            TRACE_1( STREAMMANAGER , "Stream: %s has been removed." , name );
        }
        else
        {
            TRACE_ERROR( STREAMMANAGER , "Stream: %s has not been found !" , name );

            status = PC_ERROR;
        }
    }

    return status;
}

int getStreamUrlFromName( const char *name )
{
    TRACE_2( STREAMMANAGER , "getStreamUrlFromName( %s )." , name );

    int i;

    for( i = 0 ; i < MAX_STREAM ; i++ )
    {
        if( streamProps[i].name == NULL )
        {
            continue;
        }
        else if( strcmp( streamProps[i].name , name ) == 0 )
        {
            TRACE_1( STREAMMANAGER , "Getting stream : %s." , name );

            return i;
        }
        else
        {
            TRACE_ERROR( STREAMMANAGER , "Cannot get stream : %s." , name );
        }
    }

    return PC_ERROR;
}
