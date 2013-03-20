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

static int initVlc( void )
{
    TRACE_2( VLCMANAGER , "initVlc().");

    int status = PC_SUCCESS;

    vlcInstance = libvlc_new( 0 , NULL );

    if( vlcInstance == NULL )
    {
        TRACE_ERROR( VLCMANAGER , "Fail to create a new vlc instance.");

        status = PC_ERROR;
    }

    return status;
}

int playUrl( char *url )
{
    TRACE_2( VLCMANAGER , "playUrl( %s )." , url );

    int status = PC_SUCCESS;

    char fullurl[255];
    libvlc_media_player_t *vlcMediaPlayer = NULL;

    if( vlcInstance == NULL )
    {
        if( initVlc() == PC_ERROR )
        {
            return PC_ERROR;
        }
    }

    memset( fullurl , 0 , 255 );

    snprintf( fullurl , 255 , "http://%s:1337/%s" , dispatcherIp , url );

    vlcMedia = libvlc_media_new_path( vlcInstance , fullurl );

    if( vlcMedia != NULL )
    {
        vlcMediaPlayer = libvlc_media_player_new_from_media( vlcMedia );

        if( vlcMediaPlayer != NULL )
        {
            TRACE_INFO( VLCMANAGER , "Playing url: %s" , fullurl );

            libvlc_media_release( vlcMedia );

            libvlc_media_player_play( vlcMediaPlayer );
        }
        else
        {
            TRACE_ERROR( VLCMANAGER , "Cannot create vlcMediaPlayer.");

            status = PC_ERROR;
        }
    }
    else
    {
        TRACE_ERROR( VLCMANAGER , "Cannot create vlcMedia.");

        status = PC_ERROR;
    }

    return status;
}

void destroyVlc( void )
{
    TRACE_2( VLCMANAGER , "destroyVlc().");

    libvlc_release( vlcInstance );
}
