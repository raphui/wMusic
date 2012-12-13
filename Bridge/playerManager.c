#include "playerManager.h"

static sp_track *track;
static audio_fifo_t g_audiofifo;

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


int createTrackFromUrl( char *url )
{
    TRACE_2( PLAYERMANAGER , "createTrackFromUrl( %s )" , url );

    sp_link *link;
    sp_error error;

    TRACE_1( PLAYERMANAGER , "Creating URL : %s..." , url );

    link = sp_link_create_from_string( url );

    if( link == NULL )
    {
        TRACE_ERROR( PLAYERMANAGER , "Fail to create link.");

        return PC_ERROR;
    }
    else
    {
        TRACE_3( PLAYERMANAGER , "Success to create link.");
    }

    TRACE_1( PLAYERMANAGER , "Construct track...");

    track = sp_link_as_track( link );

    if( track == NULL )
    {
        TRACE_ERROR( PLAYERMANAGER , "Fail to create track.");

        return PC_ERROR;
    }
    else
    {
        TRACE_3( PLAYERMANAGER , "Success to create track.");
    }

    error = sp_track_add_ref( track );

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( PLAYERMANAGER , "Cannot add ref track, reason: %s" , sp_error_message( error ) );
    }

    sp_link_release( link );

    audio_init( &g_audiofifo );

    running = 1;
    playing = 0;

    return PC_SUCCESS;
}

int createTrackFromUri( char *uri )
{
    TRACE_2( PLAYERMANAGER , "createTrackFromUri( %s )" , uri );

    sp_link *link;
    sp_error error;

    createFile();

    TRACE_1( PLAYERMANAGER , "Creating URI : %s..." , uri );

//    link = sp_link_create_from_string( uri );

    link = sp_link_create_from_string("spotify:track:0RvMG0PBy3uMRz2nULfjsK");

//    link = sp_link_create_from_string("spotify:track:1mzn6CQ71eUmgCSfbVmicN");

    if( link == NULL )
    {
        TRACE_ERROR( PLAYERMANAGER , "Fail to create link.");

        return PC_ERROR;
    }
    else
    {
        TRACE_3( PLAYERMANAGER , "Success to create link.");
    }

    TRACE_1( PLAYERMANAGER , "Construct track...");

    track = sp_link_as_track( link );

    if( track == NULL )
    {
        TRACE_ERROR( PLAYERMANAGER , "Fail to create track.");

        return PC_ERROR;
    }
    else
    {
        TRACE_3( PLAYERMANAGER , "Success to create track.");
    }

    error = sp_track_add_ref( track );

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( PLAYERMANAGER , "Cannot add ref track, reason: %s" , sp_error_message( error ) );
    }

    sp_link_release( link );

    audio_init( &g_audiofifo );

    running = 1;
    playing = 0;

    return PC_SUCCESS;
}

int loadMusic( sp_session *session, char *uri )
{
    TRACE_2( PLAYERMANAGER , "loadMusic().");

    int status = PC_SUCCESS;

    pthread_mutex_lock( &mutexSession );

    if( createTrackFromUri( uri ) == PC_ERROR )
        status = PC_ERROR;

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

    error = sp_session_player_play( session , 1 );

    if( firstTime++ != 0 )
        playStream("rtpStreaming");

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( PLAYERMANAGER , "Cannot play track, reason: %s" , sp_error_message( error ) );

        status = PC_ERROR;
    }
    else
    {
       TRACE_3( PLAYERMANAGER , "Success to play track.");
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

    if( track != NULL )
    {
        TRACE_3( PLAYERMANAGER , "Track name: %s" , sp_track_name( track ) );

        loadTrack( session , track );

    }
}


void end_of_track( sp_session *session )
{
    TRACE_2( PLAYERMANAGER , "end_of_track().");

    TRACE_3( PLAYERMANAGER , "End of track...");

    audio_fifo_flush( &g_audiofifo );

    sp_session_player_play( session , 0 );
    sp_session_player_unload( session );

    playing = 0;
}

int music_delivery( sp_session *session , const sp_audioformat *format , const void *frames , int num_frames )
{
    TRACE_2( PLAYERMANAGER , "music_delivery().");

    TRACE_3( PLAYERMANAGER , "Playing music...%d" , num_frames );

    audio_fifo_t *af = &g_audiofifo;
    audio_fifo_data_t *afd;
    size_t s;

    int16_t *data;

//    sendControlMulticast("START");

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
    afd = malloc( sizeof( *afd ) + s );

    afd->channels = format->channels;
//    sendVoidMulticast( &afd->channels , sizeof( int ) );

    afd->rate = format->sample_rate;
//    sendVoidMulticast( &afd->rate , sizeof( int ) );

    afd->nsamples = num_frames;
//    sendVoidMulticast( &afd->nsamples , sizeof( int ) );

    memcpy( afd->samples , frames , s );

    data = ( int16_t * )malloc( s );

    memcpy( data , afd->samples , s );

//    sendVoidMulticast( data , s );

    TAILQ_INSERT_TAIL( &af->q , afd , link );
    af->qlen += num_frames;

    pthread_cond_signal( &af->cond );
    pthread_mutex_unlock( &af->mutex );


    writeFile( &afd->samples );

//    sendControlMulticast("STOP");

    return num_frames;
}
