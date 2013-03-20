/*  wMusic - Music system software
 *  Copyright (C) 2013  Raphaël POGGI
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
        return loadMusic( g_session , url , name );
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

            if( pausing == FALSE )
                status = playMusic( g_session , streamProps[index].url , name );
            else if( pausing == TRUE )
                status = pauseStreamer( name );

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
            status = pauseMusic( g_session , streamProps[index].url , name );
        }
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

    sizeUrl = strlen( url ) + 1;
    sizeName = strlen( name ) + 1;

    TRACE_3( STREAMMANAGER , "Size name: %d , size url: %d."  , sizeName , sizeUrl );

    for( i = 0 ; i < MAX_STREAM ; i++ )
    {
        if( ( streamProps[i].name == NULL ) || ( strcmp( streamProps[i].name , "" ) == 0 ) )
        {
            TRACE_3( STREAMMANAGER , "Settings the properties.");

//            streamProps[i].url = ( char * )zmalloc( sizeUrl * sizeof( char ) );
//            streamProps[i].name = ( char * )zmalloc( sizeName * sizeof( char ) );

//            memset( streamProps[i].url , 0 , sizeUrl );
//            memset( streamProps[i].name , 0 , sizeName );

            streamProps[i].url = ( char * )zmalloc( sizeUrl * sizeof( char ) );
            streamProps[i].name = ( char * )zmalloc( sizeName * sizeof( char ) );

            strncpy( streamProps[i].name , name , sizeName );
            strncpy( streamProps[i].url , url , sizeUrl );

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
