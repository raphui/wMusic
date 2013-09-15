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

#include "stream.h"

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

    int stream = getStreamUrlFromName( name );

    if( stream == PC_ERROR )
    {
        TRACE_3( STREAMMANAGER , "Stream is not existing.");

        initPlayerEnv();

        registerNewStream( url , name );

        stream = getStreamUrlFromName( name );
    }

    if( strstr( url , "spotify") == NULL )
    {
        return loadStreamFromUrl( url , name );
    }
    else
    {
        return loadMusic( g_session , url , name , streamProps[stream].playqueue );
    }

}

int playStreamer( char *name )
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
                status = playMusic( g_session , streamProps[index].url , name , streamProps[index].playqueue );
            else if( pausing == TRUE )
                status = pauseStreamer( name );

        }
    }

    return status;
}

int pauseStreamer( char *name )
{
    TRACE_2( STREAMMANAGER , "pauseStream( %s )." , name );

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

int nextTrackInStream( char *name )
{
    TRACE_2( STREAMMANAGER , "nextTrackInStream( %s )." , name );

    int status = PC_SUCCESS;

    int index = getStreamUrlFromName( name );

    char response[255] = { 0 };

    if( index == PC_ERROR )
    {
        TRACE_ERROR( STREAMMANAGER , "Fail to retrieve stream.");

        status = PC_ERROR;

        snprintf( response , 255 , "Fail to retrieve stream." );

        sendVoid( ( void * )response , strlen( response ) );
    }
    else
    {
        if( hasNextTrackFromPlayqueue( streamProps[index].playqueue ) == TRUE )
        {
            playMusic( g_session , streamProps[index].url , name , streamProps[index].playqueue );
        }
        else
        {
            TRACE_WARNING( STREAMMANAGER , "Playqueue of stream: %s is empty !" , name );

            status = PC_ERROR;

            snprintf( response , 255 , "Playqueue of stream: %s is empty !" , name );

            sendVoid( ( void * )response , strlen( response ) );
        }
    }


    return status;
}

int loadPlaylistInStream( char *playlist , char *name )
{
    TRACE_2( STREAMMANAGER , "loadPlaylistInStream( %s , %s )." , playlist , name );

    int status = PC_SUCCESS;

    int i = 0;

    char response[255] = { 0 };

    sp_playlist *pl = NULL;

    pl = getPlaylist( atoi( playlist ) );

    if( pl == NULL )
    {
        TRACE_ERROR( STREAMMANAGER , "Cannot get playlist: %s" , playlist );

        status = PC_ERROR;

        snprintf( response , 255 , "NOK: Cannot get playlist: %s" , playlist );

        sendVoid( ( void * )response , strlen( response ) );
    }
    else
    {
        for( i = 0 ; i < sp_playlist_num_tracks( pl ) ; i++ )
        {
            char uri[255] = {0};

            LOCK_MUTEX( PLAYLISTMANAGER , &mutexSession );

            sp_link *link = sp_link_create_from_track( sp_playlist_track( pl , i ) , 0  );

            sp_link_as_string( link , uri , 255 );

            UNLOCK_MUTEX( PLAYLISTMANAGER , &mutexSession );

            TRACE_3( STREAMMANAGER , "Get track: %s from playlist: %s" , uri , playlist );

            loadStreamer( uri , name );

        }

        snprintf( response , 255 , "OK");

        sendVoid( ( void * )response , strlen( response ) );
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

            streamProps[i].url = ( char * )zmalloc( sizeUrl * sizeof( char ) );
            streamProps[i].name = ( char * )zmalloc( sizeName * sizeof( char ) );

            strncpy( streamProps[i].name , name , sizeName );
            strncpy( streamProps[i].url , url , sizeUrl );

            streamProps[i].playqueue = getPlayqueue();

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

void getInfosCurrentTrack( char *name )
{
    TRACE_2( STREAMMANAGER , "getInfosCurrentTrack( %s )." , name );

    char *infos = getTrackInfos();

    sendVoid( ( void * )infos , strlen( infos ) );
}

void getListStream( void )
{
    TRACE_2( STREAMMANAGER , "getListStream()");

    int i = 0;
    int flags = FALSE;

    char response[255] = { 0 };

    for( i =  0 ; i < MAX_STREAM ; i++ )
    {
        if( streamProps[i].name != NULL && strcmp( streamProps[i].name  , "" ) != 0 )
        {
            strcat( response , streamProps[i].name );
            strcat( response , ",");

            flags = TRUE;
        }
    }

    if( flags == FALSE )
    {
        snprintf( response , 255 , "none");
    }

    sendVoid( ( void * )response , strlen( response ) );

}
