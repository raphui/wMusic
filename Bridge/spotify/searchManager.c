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

#include "searchManager.h"

static void searchComplete( sp_search *search , void *userdata );

int search( sp_session *session , char *query )
{
    TRACE_2( SEARCHMANAGER , "search( __session__ , %s )." , query );

    LOCK_MUTEX( SEARCHMANAGER , &mutexSession );

    sp_search_create( session , query , 0 , MAX_SEARCH_COUNT , 0 , MAX_SEARCH_COUNT , 0 , MAX_SEARCH_COUNT , 0 , MAX_SEARCH_COUNT , SP_SEARCH_STANDARD , &searchComplete , NULL );

    UNLOCK_MUTEX( SEARCHMANAGER , &mutexSession );

    return PC_SUCCESS;
}

static void searchComplete( sp_search *search , void *userdata )
{
    TRACE_2( SEARCHMANAGER , "searchComplete()");

    int i = 0;
    int trackDur;
    int searchCount;
    int sizeToBeSend;

    char uri[255];
    char start[] = "<results>";
    char stop[] = "</results>";
    char *toBeSend;

    sp_track *currentTrack;

//    LOCK_MUTEX( SEARCHMANAGER , &mutexSession );

    if( sp_search_error( search ) == SP_ERROR_OK )
    {
        TRACE_1( SEARCHMANAGER , "Query: %s" , sp_search_query( search ) );

        searchCount = sp_search_num_tracks( search );

        sizeToBeSend = 512 * searchCount;

        toBeSend = ( char * )zmalloc( sizeToBeSend );

        memset( toBeSend , 0 , sizeToBeSend );

        strcat( toBeSend , start );

        for( i = 0 ; i <  searchCount ; i++ )
        {
            currentTrack = sp_search_track( search , i );

            if( ( currentTrack == NULL ) )
            {
                TRACE_WARNING( SEARCHMANAGER , "currentTrack or currentArtist or currentAlbum is NULL.");

                continue;
            }

            trackDur = trackDuration( currentTrack );

            sprintf( toBeSend + strlen( toBeSend ) , "<result><uri>%s</uri><track>%s</track><artist>%s</artist><time>%d</time><album>%s</album></result>"
                                , trackUri( currentTrack , uri )
                                , printTrack( currentTrack )
                                , sp_artist_name( sp_track_artist( currentTrack , 0 ) )
                                , trackDur
                                , sp_album_name( sp_track_album( currentTrack ) ) );
        }

        strcat( toBeSend , stop );

        sendVoid( toBeSend , sizeToBeSend );

        zfree( toBeSend );
    }

    sp_search_release( search );

//    UNLOCK_MUTEX( SEARCHMANAGER , &mutexSession );
}

const char *printAlbum( sp_album *album )
{
    TRACE_2( SEARCHMANAGER , "printAlbum()");

//    LOCK_MUTEX( SEARCHMANAGER , &mutexSession );

    const char *albumName;

    albumName = sp_album_name( album );

    TRACE_3( SEARCHMANAGER , "Album:\t\t %s" , albumName );

//    UNLOCK_MUTEX( SEARCHMANAGER , &mutexSession );

    return albumName;
}

const char *printArtist( sp_artist *artist )
{
    TRACE_2( SEARCHMANAGER , "printArtist()");

//    LOCK_MUTEX( SEARCHMANAGER , &mutexSession );

    const char *artistName;

    artistName = sp_artist_name( artist );

    TRACE_3( SEARCHMANAGER , "Artist:\t\t %s" , artistName );

//    UNLOCK_MUTEX( SEARCHMANAGER , &mutexSession );

    return artistName;
}

const char *printTrack( sp_track *track )
{
    TRACE_2( SEARCHMANAGER , "printTrack()");

//    LOCK_MUTEX( SEARCHMANAGER , &mutexSession );

    const char *trackName;

    trackName = sp_track_name( track );

    TRACE_3( SEARCHMANAGER , "Track:\t\t %s" , trackName );

//    UNLOCK_MUTEX( SEARCHMANAGER , &mutexSession );

    return trackName;
}

int trackDuration( sp_track *track )
{
    TRACE_2( SEARCHMANAGER , "trackDuration()");

//    LOCK_MUTEX( SEARCHMANAGER , &mutexSession );

    int duration = sp_track_duration( track );

//    UNLOCK_MUTEX( SEARCHMANAGER , &mutexSession );

    return duration;
}

char *trackUri( sp_track *track , char *uri )
{
    TRACE_2( SEARCHMANAGER , "trackUri()");

//    LOCK_MUTEX( SEARCHMANAGER , &mutexSession );

    sp_link_as_string( sp_link_create_from_track( track , 0 ) , uri , 255 );

//    UNLOCK_MUTEX( SEARCHMANAGER , &mutexSession );

    return uri;
}
