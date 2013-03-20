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

#include "vlcManager.h"

static libvlc_instance_t *vlcInstance = NULL;
static libvlc_media_t *vlcMedia = NULL;
static char *currentUrl = NULL;

static int initVlc( void )
{
    TRACE_2( VLCMANAGER , "initVlc()");

    int status = PC_SUCCESS;

    vlcInstance = libvlc_new( 0 , NULL );

    if( vlcInstance == NULL )
    {
        TRACE_ERROR( VLCMANAGER , "Fail to create a new vlc instance.");

        status = PC_ERROR;
    }

    currentUrl = ( char * )zmalloc( 124 * sizeof( char ) );

    return status;
}

int streamFile( char *filename )
{
    TRACE_2( VLCMANAGER , "streamFile( %s )" , filename );

    int status = PC_SUCCESS;
    int ret;
    char output[255];
    char *tmp;
    char name[255];

    memset( output , 0 , 255 );
    memset( name , 0 , 255 );

    tmp = strrchr( filename , '/');

    while( *( ( tmp++ + 1 ) ) != '.' )
        memcpy( name + strlen( name ) , tmp , 1 );

//    sprintf( output , "#http{dst=:1337/test3.wav}");
    sprintf( output , "#transcode{vcodec=drac,vb=800,scale=1,acodec=mpga,ab=128,channels=2,samplerate=44100}:http{dst=:1337/%s.mp3}" , name );

    TRACE_INFO( VLCMANAGER , "Url stream : http:1337/%s.mp3" , name );

    if( vlcInstance == NULL )
        ret = initVlc();

    if( ret == PC_ERROR )
    {
        TRACE_ERROR( VLCMANAGER , "Fail to init VLC , cannot stream the file !");

        status = PC_ERROR;
    }
    else
    {
        vlcMedia = libvlc_media_new_path( vlcInstance , filename );

        if( vlcMedia != NULL )
        {

            ret = libvlc_vlm_add_broadcast( vlcInstance , name , filename , output , 0 , NULL , 1 , 0 );

            if( ret < 0 )
            {
                TRACE_ERROR( VLCMANAGER , "Cannot add a broadcast.");

                status = PC_ERROR;
            }
            else
            {
                TRACE_3( VLCMANAGER , "Broadcast stream added.");

                ret = libvlc_vlm_play_media( vlcInstance , name );

                if( ret < 0 )
                {
                    TRACE_ERROR( VLCMANAGER , "Cannot diffuse the stream.");

                    status = PC_ERROR;
                }
                else
                {
                    TRACE_1( VLCMANAGER , "Start diffuse the stream.");
                }
            }
        }
        else
        {
            TRACE_ERROR( VLCMANAGER , "Fail to create a new media from the path: %s" , filename );

            status = PC_ERROR;
        }
    }

    return status;
}


int destroyVlc( void )
{
    TRACE_2( VLCMANAGER , "destroyVlc()");

    int status = PC_SUCCESS;

    if( vlcInstance != NULL )
        libvlc_release( vlcInstance );

    zfree( currentUrl );

    return status;
}

int loadStreamFromUrl( char *url , char *name )
{
    TRACE_2( VLCMANAGER , "loadStreamFromUrl( %s , %s )." , url , name );

    int status = PC_SUCCESS;
    int ret;
    char output[255];

    memset( output , 0 , 255 );

    sprintf( output , "#transcode{vcodec=drac,vb=800,scale=1,acodec=mpga,ab=128,channels=2,samplerate=44100}:http{dst=:1337/%s.mp3}" , name );

    if( vlcInstance == NULL )
        ret = initVlc();

    if( ret == PC_ERROR )
    {
        TRACE_ERROR( VLCMANAGER , "Fail to init VLC , cannot stream the file !");

        status = PC_ERROR;
    }
    else
    {
        strncpy( currentUrl , url , strlen( url ) );

        registerNewStream( url , name );

        vlcMedia = libvlc_media_new_path( vlcInstance , url );

        if( vlcMedia != NULL )
        {

            ret = libvlc_vlm_add_broadcast( vlcInstance , name , url , output , 0 , NULL , 1 , 0 );

            if( ret < 0 )
            {
                TRACE_ERROR( VLCMANAGER , "Cannot add a broadcast.");

                status = PC_ERROR;
            }
            else
            {
                TRACE_3( VLCMANAGER , "Broadcast stream added.");

                ret = libvlc_vlm_play_media( vlcInstance , name );

                if( ret < 0 )
                {
                    TRACE_ERROR( VLCMANAGER , "Cannot diffuse the stream.");

                    status = PC_ERROR;
                }
                else
                {
                    TRACE_1( VLCMANAGER , "Start diffuse the stream.");
                }
            }
        }
        else
        {
            TRACE_ERROR( VLCMANAGER , "Fail to create a new media from the path: %s" , url );

            status = PC_ERROR;
        }
    }

    return status;
}

int playStream( const char *name )
{
    TRACE_2( VLCMANAGER , "playStream( %s )" , name );

    int status = PC_SUCCESS;
    int ret;

    ret = libvlc_vlm_play_media( vlcInstance , name );

    if( ret < 0 )
    {
        TRACE_ERROR( VLCMANAGER , "Cannot play the stream.");

        status = PC_ERROR;
    }
    else
    {
        TRACE_1( VLCMANAGER , "Play the stream.");
    }

    return status;
}

int pauseStream( const char *name )
{
    TRACE_2( VLCMANAGER , "pauseStream( %s )" , name );

    int status = PC_SUCCESS;
    int ret;

    ret = libvlc_vlm_pause_media( vlcInstance , name );

    if( ret < 0 )
    {
        TRACE_ERROR( VLCMANAGER , "Cannot pause the stream.");

        status = PC_ERROR;
    }
    else
    {
        TRACE_1( VLCMANAGER , "Pause the stream.");
    }

    return status;
}
