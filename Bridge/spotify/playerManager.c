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

#include "playerManager.h"

static sp_track *currentTrack;
static audio_fifo_t g_audiofifo;
static char currentStreamName[255] = { 0 };


void initPlayerEnv( void )
{
    TRACE_2( PLAYERMANAGER , "initPlayerEnv().");

    static int firstTime = 0;

    //We have to init the audio driver only 1 time !
    if( firstTime == 0 )
    {
        audio_init( &g_audiofifo );
        firstTime++;
    }

    initPlayqueue();
}

static int loadTrack( sp_session *session , sp_track *track )
{
    TRACE_2( PLAYERMANAGER , "loadTrack().");

    int status = PC_SUCCESS;

    sp_error error;

    error = sp_session_player_load( session , track );

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( PLAYERMANAGER , "Cannot load track, reason: %s" , sp_error_message( error ) );

        status = PC_ERROR;
    }
    else
    {
        TRACE_1( PLAYERMANAGER , "Track loaded.");
    }

    return status;
}

int createTrackFromUri( char *uri , char *name )
{
    TRACE_2( PLAYERMANAGER , "createTrackFromUri( %s , __track__ )" , uri );

    sp_link *link;
    sp_error error;

    if( playing == FALSE && hasNextTrack() == FALSE )
        createFile( name );

    TRACE_1( PLAYERMANAGER , "Creating URI : %s" , uri );

    link = sp_link_create_from_string( uri );

    if( link == NULL )
    {
        TRACE_ERROR( PLAYERMANAGER , "Fail to create link.");

        return PC_ERROR;
    }
    else
    {
        TRACE_1( PLAYERMANAGER , "Success to create link.");
    }

    TRACE_3( PLAYERMANAGER , "Construct track...");

    currentTrack = sp_link_as_track( link );

    if( currentTrack == NULL )
    {
        TRACE_ERROR( PLAYERMANAGER , "Fail to create track.");

        return PC_ERROR;
    }
    else
    {
        TRACE_1( PLAYERMANAGER , "Success to create track.");
    }

    error = sp_track_add_ref( currentTrack );

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( PLAYERMANAGER , "Cannot add ref track, reason: %s" , sp_error_message( error ) );

        return PC_ERROR;
    }

    sp_link_release( link );

    running = TRUE;
//    playing = FALSE;

    return PC_SUCCESS;
}

int loadMusic( sp_session *session, char *uri , char *name , playqueue_fifo_t *playqueue )
{
    TRACE_2( PLAYERMANAGER , "loadMusic().");

    int status = PC_SUCCESS;
    char response[255] = { 0 };

    /* If it's the first time we load a track, we have to init the audio driver and the playqueue */
//    if( firstTime++ == 0 )
//        initPlayerEnv();

    LOCK_MUTEX( PLAYERMANAGER , &mutexSession );

    sp_track *track = NULL;

    currentTrack = track;

    if( createTrackFromUri( uri , name ) == PC_ERROR )
        status = PC_ERROR;

    if( currentTrack != NULL)
    {
        TRACE_1( PLAYERMANAGER , "Adding track to the playlist.");

        addTracksToPlayqueue( playqueue , currentTrack );

        snprintf( response , 255 , "OK");

        sendVoid( ( void * )response , strlen( response ) );
    }
    else
    {
        TRACE_ERROR( PLAYERMANAGER , "Cannot add track to the playlist because track is NULL.");

        status = PC_ERROR;

        snprintf( response , 255 , "NOK: Cannot add the track to the playlist because the URI might be invalid.");

        sendVoid( ( void * )response , strlen( response ) );
    }

    UNLOCK_MUTEX( PLAYERMANAGER , &mutexSession );

    return status;
}

int playMusic( sp_session *session , char *uri , char *name , playqueue_fifo_t *playqueue )
{
    TRACE_2( PLAYERMANAGER , "playMusic().");

    static int firstTime = 0;

    int status = PC_SUCCESS;

    char response[255] = { 0 };

    sp_error error;

    LOCK_MUTEX( PLAYERMANAGER , &mutexSession );

    TRACE_3( PLAYERMANAGER , "Test if a music is playing or not");

    if( currentTrack == NULL )
    {
        TRACE_WARNING( PLAYERMANAGER , "Cannot play track because no track has been loaded.");

        status = PC_ERROR;

        snprintf( response , 255 , "NOK: Cannot play track because no track has been loaded.");

        sendVoid( ( void * )response , strlen( response ) );
    }
    else
    {
        TRACE_1( PLAYERMANAGER , "Getting the track.");


        currentTrack = getNextTrackToPlayqueue( playqueue );

        loadTrack( session ,  currentTrack );

        error = sp_session_player_play( session , 1 );

        if( error != SP_ERROR_OK )
        {
            TRACE_ERROR( PLAYERMANAGER , "Cannot play track, reason: %s" , sp_error_message( error ) );

            status = PC_ERROR;
        }
        else
        {
           TRACE_1( PLAYERMANAGER , "Success to play track.");

           if( firstTime++ != 0 )
               playStream( name );

           playing = TRUE;

           snprintf( response , 255 , "OK");

           sendVoid( ( void * )response , strlen( response ) );
        }

        if( currentStreamName[0] == 0 )
        {
            strncpy( currentStreamName , name , strlen( name ) );
        }
        else if( strcmp( currentStreamName , name ) != 0 )
        {
            memset( currentStreamName , 0 , 255 );
            strncpy( currentStreamName , name , strlen( name ) );
        }
        else if( !strcmp( currentStreamName , name ) )
        {
            //Do nothing
        }


    }

    UNLOCK_MUTEX( PLAYERMANAGER , &mutexSession );


    return status;
}

int pauseMusic(sp_session *session , char *uri , char *name )
{
    TRACE_2( PLAYERMANAGER , "pauseMusic().");

    int status = PC_SUCCESS;

    char response[255] = { 0 };

    LOCK_MUTEX( PLAYERMANAGER , &mutexSession );

    sp_error error;

    if( pausing == FALSE )
    {
        pauseStream( name );

        error = sp_session_player_play( session , 0 );

        if( error != SP_ERROR_OK )
        {
            TRACE_ERROR( PLAYERMANAGER , "Cannot pause track, reason: %s" , sp_error_message( error ) );

            status = PC_ERROR;

            snprintf( response , 255 , "NOK: Cannot pause track, reason: %s" , sp_error_message( error ) );

            sendVoid( ( void * )response , strlen( response ) );
        }
        else
        {
            TRACE_1( PLAYERMANAGER , "Success to pause track.");

            pausing = TRUE;

            snprintf( response , 255 , "OK");

            sendVoid( ( void * )response , strlen( response ) );

        }
    }
    else if( pausing == TRUE )
    {
        playStream( name );

        error = sp_session_player_play( session , 1 );

        if( error != SP_ERROR_OK )
        {
            TRACE_ERROR( PLAYERMANAGER , "Cannot play track, reason: %s" , sp_error_message( error ) );

            status = PC_ERROR;
        }
        else
        {
            TRACE_1( PLAYERMANAGER , "Success to play track.");

            pausing = FALSE;
        }
    }

    UNLOCK_MUTEX( PLAYERMANAGER , &mutexSession );

    return status;
}

int nextMusic( sp_session *session , char *uri )
{
    TRACE_2( PLAYERMANAGER , "nextMusic( __session__ , %s )." , uri );

    int status = PC_SUCCESS;

    TRACE_3( PLAYERMANAGER , "Getting next track from the playqueue.");

    if( hasNextTrack() == TRUE )
    {
        TRACE_1( PLAYERMANAGER , "Playing next track.");

//        playMusic( g_session , NULL );
    }
    else
    {
        TRACE_ERROR( PLAYERMANAGER , "No next track in the playqueue.");

        status = PC_ERROR;
    }

    return status;
}


void metadata_updated( sp_session *session )
{
    TRACE_2( PLAYERMANAGER , "metadata_updated().");

}


void end_of_track( sp_session *session )
{
    TRACE_2( PLAYERMANAGER , "end_of_track().");

    TRACE_3( PLAYERMANAGER , "End of track...");

//    LOCK_MUTEX( PLAYERMANAGER , &mutexSession );

    TRACE_3( PLAYERMANAGER , "Removing the track which have been played.");

    sp_track_release( currentTrack );

//    UNLOCK_MUTEX( PLAYERMANAGER , &mutexSession );

//    if( hasNextTrack() == TRUE )
//    {
//        TRACE_1( PLAYERMANAGER , "Load next music !");

//        playMusic( session , "" , currentStreamName );
//    }
    if( nextTrackInStream( currentStreamName ) == PC_SUCCESS )
    {
        TRACE_1( PLAYERMANAGER , "Load next music !");
    }
    else
    {
        TRACE_WARNING( PLAYERMANAGER , "No more music in the mainplaylist");

        audio_fifo_flush( &g_audiofifo );

        LOCK_MUTEX( PLAYERMANAGER , &mutexSession );

        sp_session_player_play( session , 0 );
        sp_session_player_unload( session );

        UNLOCK_MUTEX( PLAYERMANAGER , &mutexSession );

        playing = FALSE;
    }



}

int music_delivery( sp_session *session , const sp_audioformat *format , const void *frames , int num_frames )
{
    TRACE_2( PLAYERMANAGER , "music_delivery().");

    TRACE_3( PLAYERMANAGER , "Playing music...%d" , num_frames );

    audio_fifo_t *af = &g_audiofifo;
    audio_fifo_data_t *afd;
    size_t s;

    TRACE_3( PLAYERMANAGER , "###############################################");
    TRACE_3( PLAYERMANAGER , "Channels: %d" , format->channels );
    TRACE_3( PLAYERMANAGER , "Rate: %d" , format->sample_rate );
    TRACE_3( PLAYERMANAGER , "NSamples: %d" , num_frames );
    TRACE_3( PLAYERMANAGER , "###############################################");

    // audio discontinuity, do nothing
    if( num_frames == 0 )
    {
        pthread_mutex_unlock( &af->mutex );
        return 0;
    }

    // buffer one second of audio
    if( af->qlen > format->sample_rate )
        return 0;

    s = num_frames * sizeof( int16_t ) * format->channels;
    afd = zmalloc( sizeof( *afd ) + s );

    afd->channels = format->channels;

    afd->rate = format->sample_rate;

    afd->nsamples = num_frames;

    memcpy( afd->samples , frames , s );

    TAILQ_INSERT_TAIL( &af->q , afd , link );
    af->qlen += num_frames;

    pthread_cond_signal( &af->cond );
    pthread_mutex_unlock( &af->mutex );

    return num_frames;
}

char *getTrackInfos( void )
{
    TRACE_2( PLAYERMANAGER , "getTrackInfo().");

    char *buff = ( char * )zmalloc( 512 * sizeof( char ) );

    LOCK_MUTEX( PLAYERMANAGER , &mutexSession );


    //If the return of sp_track_name is a empty string, then an error occured
    if( strcmp( sp_track_name( currentTrack ) , "" ) == 0 )
    {
        snprintf( buff , 512 , "Cannot get metadata from track.");
    }
    else
    {
        sprintf( buff , "%s , %s , %s" , sp_track_name( currentTrack ) , sp_artist_name( sp_track_artist( currentTrack , 0 ) ) , sp_album_name( sp_track_album( currentTrack ) ) );
    }

    UNLOCK_MUTEX( PLAYERMANAGER , &mutexSession );

    return buff;
}
