#include "playerManager.h"



int play( sp_session *session , char *uri )
{
    TRACE_2( SPOTIFYMANAGER , "play( %s )" , uri );

    sp_link *link;
    sp_error error;
    int next_timeout;

    printf("Creating URI : %s...\n" , uri );

//    link = sp_link_create_from_string( uri );

    link = sp_link_create_from_string("spotify:track:0hErXRUxNluo55B4djV9pU");

    if( link == NULL )
    {
        printf("Fail to create link.\n");

        return -1;
    }
    else
    {
        printf("Success to create link.\n");
    }

    printf("Construct track...\n");

    track = sp_link_as_track( link );

    if( track == NULL )
    {
        printf("Fail to create track.\n");

        return -1;
    }
    else
    {
        printf("Success to create track.\n");
    }

    error = sp_track_add_ref( track );

    if( error != SP_ERROR_OK )
    {
        printf("Cannot add ref track, reason: %s" , sp_error_message( error ) );
    }

    sp_link_release( link );

    audio_init( &g_audiofifo );

    running = 1;
    playing = 0;

//    while( running )
//    {
//        sp_session_process_events( session , &next_timeout );

//        if( playing == 0 )
//        {
//            printf("Let's play the music !\n");


//            playing = 1;
//        }
//    }
}

static void playMusic( sp_session *sp , sp_track *track )
{
    TRACE_2( SPOTIFYMANAGER , "play().");

    printf("Loading track...\n");

    sp_error error;

    error = sp_session_player_load( sp , track );

    if( error != SP_ERROR_OK )
    {
        printf("Cannot load track, reason: %s\n" , sp_error_message( error ) );

        return;
    }
    else
    {
        printf("Track loaded.\n");
    }

    error = sp_session_player_play( sp , 1 );

    if( error != SP_ERROR_OK )
    {
        printf("Cannot play track, reason: %s\n" , sp_error_message( error ) );
    }
    else
    {
        printf("Success to play track.\n");
    }

}

void metadata_updated( sp_session *session )
{
    TRACE_2( SPOTIFYMANAGER , "metadata_updated().");

    if( track != NULL )
    {
        printf("Track name: %s\n" , sp_track_name( track ) );

        playMusic( session , track );

    }
}


void end_of_track( sp_session *session )
{
    TRACE_2( SPOTIFYMANAGER , "end_of_track().");

    printf("End of track...\n");

    audio_fifo_flush( &g_audiofifo );
    sp_session_player_play( session , 0 );
    sp_session_player_unload( session );

    playing = 0;
}

int music_delivery( sp_session *session , const sp_audioformat *format , const void *frames , int num_frames )
{
    TRACE_2( SPOTIFYMANAGER , "music_delivery().");

//    printf("Playing music...%d\n" , num_frames );

    audio_fifo_t *af = &g_audiofifo;
    audio_fifo_data_t *afd;
    size_t s;

    int16_t *data;

    sendControl("START");

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
    sendVoid( afd->channels , sizeof( int ) );

    afd->rate = format->sample_rate;
//    sendVoid( afd->rate , sizeof( int ) );
    sendVoid( 44100 , sizeof( int ) );

    afd->nsamples = num_frames;
    sendVoid( afd->nsamples , sizeof( int ) );

    memcpy( afd->samples , frames , s );

    data = ( int16_t * )malloc( s );

    memcpy( data , afd->samples , s );
    sendVoid( *data , sizeof( int16_t ) );

//    data = ( char * )malloc( sizeof( char ) * s );

//    strcat( ( char )&( afd->channels ) , data );
//    strcat( ( char )&( afd->rate ) , data );
//    strcat( ( char )&( afd->nsamples ) , data );
//    strcat( ( char )afd->samples[0] , data );

//    sendVoid( data , s );

//    sendData( afd , s + sizeof( *afd ) );

    TAILQ_INSERT_TAIL( &af->q , afd , link );
    af->qlen += num_frames;

    pthread_cond_signal( &af->cond );
    pthread_mutex_unlock( &af->mutex );

    sendControl("STOP");

    return num_frames;
}
