#include "playerManager.h"

static sp_track *currentTrack;
static audio_fifo_t g_audiofifo;


static void initPlayerEnv( void )
{
    TRACE_2( PLAYERMANAGER , "initPlayerEnv().");

    audio_init( &g_audiofifo );
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
        TRACE_3( PLAYERMANAGER , "Track loaded.");
    }

    return status;
}

int createTrackFromUri( char *uri , sp_track *track )
{
    TRACE_2( PLAYERMANAGER , "createTrackFromUri( %s , __track__ )" , uri );

    sp_link *link;
    sp_error error;

    createFile();

    TRACE_3( PLAYERMANAGER , "Creating URI : %s" , uri );

    link = sp_link_create_from_string( uri );

    if( link == NULL )
    {
        TRACE_ERROR( PLAYERMANAGER , "Fail to create link.");

        return PC_ERROR;
    }
    else
    {
        TRACE_3( PLAYERMANAGER , "Success to create link.");
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
        TRACE_3( PLAYERMANAGER , "Success to create track.");
    }

    error = sp_track_add_ref( currentTrack );

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( PLAYERMANAGER , "Cannot add ref track, reason: %s" , sp_error_message( error ) );

        return PC_ERROR;
    }

    sp_link_release( link );

    running = TRUE;
    playing = FALSE;

    return PC_SUCCESS;
}

int loadMusic( sp_session *session, char *uri )
{
    TRACE_2( PLAYERMANAGER , "loadMusic().");

    static int firstTime = 0;

    int status = PC_SUCCESS;

    /* If it's the first time we load a track, we have to init the audio driver and the playqueue */
    if( firstTime++ == 0 )
        initPlayerEnv();

    pthread_mutex_lock( &mutexSession );

    sp_track *track = NULL;

    currentTrack = track;

    if( createTrackFromUri( uri , currentTrack ) == PC_ERROR )
        status = PC_ERROR;

    if( currentTrack != NULL)
    {
        TRACE_3( PLAYERMANAGER , "Adding track to the playlist.");

        addTracksPlayqueue( currentTrack );

    }
    else
    {
        TRACE_ERROR( PLAYERMANAGER , "Cannot add track to the playlist because track is NULL.");

        status = PC_ERROR;
    }

    pthread_mutex_unlock( &mutexSession );

    return status;
}

int playMusic( sp_session *session , char *uri )
{
    TRACE_2( PLAYERMANAGER , "playMusic().");

    static int firstTime = 0;

    int status = PC_SUCCESS;

    sp_error error;

    pthread_mutex_lock( &mutexSession );

    TRACE_3( PLAYERMANAGER , "Test if a music is playing or not");

    if( currentTrack == NULL )
    {
        TRACE_WARNING( PLAYERMANAGER , "Cannot play track because no track has been loaded.");

        status = PC_ERROR;
    }
    else if( playing == TRUE )
    {
        TRACE_3( PLAYERMANAGER , "A music is playing, we just have to unpause it.");

        sp_session_player_play( session , 1 );
    }
    else
    {
        TRACE_3( PLAYERMANAGER , "Getting the track.");

        loadTrack( session , getNextTrack() );

        error = sp_session_player_play( session , 1 );

        if( error != SP_ERROR_OK )
        {
            TRACE_ERROR( PLAYERMANAGER , "Cannot play track, reason: %s" , sp_error_message( error ) );

            status = PC_ERROR;
        }
        else
        {
           TRACE_3( PLAYERMANAGER , "Success to play track.");

           if( firstTime++ != 0 )
               playStream("rtpStreaming");

           playing = TRUE;
        }
    }

    pthread_mutex_unlock( &mutexSession );


    return status;
}

int pauseMusic( sp_session *session , char *uri )
{
    TRACE_2( PLAYERMANAGER , "pauseMusic().");

    int status = PC_SUCCESS;

    pthread_mutex_lock( &mutexSession );

    sp_error error;

    pauseStream("rtpStreaming");

    error = sp_session_player_play( session , 0 );

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( PLAYERMANAGER , "Cannot pause track, reason: %s" , sp_error_message( error ) );

        status = PC_ERROR;
    }
    else
    {
        TRACE_3( PLAYERMANAGER , "Success to pause track.")
    }

    pthread_mutex_unlock( &mutexSession );

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

    audio_fifo_flush( &g_audiofifo );

    pthread_mutex_lock( &mutexSession );

    TRACE_3( PLAYERMANAGER , "Removing the track which have been played.");

    sp_track_release( currentTrack );

    if( hasNextTrack() == TRUE )
    {
        TRACE_3( PLAYERMANAGER , "Load next music !");

        playMusic( session , "" );
    }
    else
    {
        TRACE_WARNING( PLAYERMANAGER , "No more music in the mainplaylist");

        sp_session_player_play( session , 0 );
        sp_session_player_unload( session );

        playing = FALSE;
    }

    pthread_mutex_unlock( &mutexSession );

}

int music_delivery( sp_session *session , const sp_audioformat *format , const void *frames , int num_frames )
{
    TRACE_2( PLAYERMANAGER , "music_delivery().");

    TRACE_3( PLAYERMANAGER , "Playing music...%d" , num_frames );

    static int firstTime = 0;

    audio_fifo_t *af = &g_audiofifo;
    audio_fifo_data_t *afd;
    size_t s;

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

//    writeFile( &afd->samples );

    return num_frames;
}
