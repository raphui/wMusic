#include "spotifyManager.h"

void launchSpotifyManager( void )
{
    TRACE_2( SPOTIFYMANAGER , "launchSpotifyManager()");

    sp_session_config spconfig;
    sp_error error;

    char username[] = USERNAME;
    char password[] = PASSWORD;

    int next_timeout = 0;

    memset( &spconfig , 0 , sizeof( spconfig ) );
    memset( &spSessionCallbacks , 0 , sizeof( spSessionCallbacks ) );

    spSessionCallbacks.logged_in = &logged_in;
    spSessionCallbacks.notify_main_thread = &notify_main_thread;
    spSessionCallbacks.end_of_track = &end_of_track;
    spSessionCallbacks.music_delivery = &music_delivery;
    spSessionCallbacks.metadata_updated = &metadata_updated;

    spconfig.api_version = SPOTIFY_API_VERSION;
    spconfig.cache_location = "/home/raphio/tmp";
    spconfig.settings_location = "/home/raphio/tmp";
    spconfig.application_key = g_appkey;
    spconfig.application_key_size = g_appkey_size;
    spconfig.user_agent = "spotify-poc";
    spconfig.callbacks = &spSessionCallbacks;

    error = sp_session_create( &spconfig , &sp );

    if( error != SP_ERROR_OK )
    {
        printf("Fail to create session, reason: %s\n" , sp_error_message( error ) );
    }
    else
    {
        printf("Success to create session\n");
    }

    if( sp != NULL )
    {
        printf("Trying to login....\n");

        login = 0;

        error = sp_session_login( sp , username , password , 0 , NULL );

        if( error != SP_ERROR_OK )
        {
            printf("Fail to login, reason: %s" , sp_error_message( error ) );

            return -1;
        }

        while( login != 1 )
        {
            usleep( next_timeout * 1000 );

            sp_session_process_events( sp , &next_timeout );
        }
    }
}

void play( char *uri )
{
    TRACE_2( SPOTIFYMANAGER , "play( %s )" , uri );

    sp_link *link;
    sp_error error;
    int next_timeout;

//    sp_session_config spconfig;
//    sp_error error;
//    sp_link *link;

//    char username[] = USERNAME;
//    char password[] = PASSWORD;

//    int next_timeout = 0;

//    memset( &spconfig , 0 , sizeof( spconfig ) );
//    memset( &spSessionCallbacks , 0 , sizeof( spSessionCallbacks ) );

//    spSessionCallbacks.logged_in = &logged_in;
//    spSessionCallbacks.notify_main_thread = &notify_main_thread;
//    spSessionCallbacks.end_of_track = &end_of_track;
//    spSessionCallbacks.music_delivery = &music_delivery;
//    spSessionCallbacks.metadata_updated = &metadata_updated;

//    spconfig.api_version = SPOTIFY_API_VERSION;
//    spconfig.cache_location = "/home/raphio/tmp";
//    spconfig.settings_location = "/home/raphio/tmp";
//    spconfig.application_key = g_appkey;
//    spconfig.application_key_size = g_appkey_size;
//    spconfig.user_agent = "spotify-poc";
//    spconfig.callbacks = &spSessionCallbacks;

//    error = sp_session_create( &spconfig , &sp );

//    if( error != SP_ERROR_OK )
//    {
//        printf("Fail to create session, reason: %s\n" , sp_error_message( error ) );
//    }
//    else
//    {
//        printf("Success to create session\n");
//    }

//    if( sp != NULL )
//    {
//        printf("Trying to login....\n");

//        login = 0;

//        error = sp_session_login( sp , username , password , 0 , NULL );

//        if( error != SP_ERROR_OK )
//        {
//            printf("Fail to login, reason: %s" , sp_error_message( error ) );

//            return -1;
//        }

//        while( login != 1 )
//        {
//            usleep( next_timeout * 1000 );

//            sp_session_process_events( sp , &next_timeout );
//        }

    printf("Creating URI...\n");

    link = sp_link_create_from_string( uri );

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

    while( running )
    {
        sp_session_process_events( sp , &next_timeout );

        if( ( login == 1 ) && ( playing == 0 ) )
        {
            printf("Let's play the music !\n");


            playing = 1;
        }
    }
}

static void logged_in( sp_session *session , sp_error error)
{

    TRACE_2( SPOTIFYMANAGER , "logged_in()");

    if( error != SP_ERROR_OK )
    {
        printf("Fail to login, reason: %s.\n" ,  sp_error_message( error ) );

        exit( 1 );
    }

    printf("Success to login.\n");

    printf("Start server...\n");

    pthread_create( &serverThread , NULL , ( void * ) &createServer , 1337 );

    login = 1;
}

static void logged_out( sp_session *session )
{
    TRACE_2( SPOTIFYMANAGER , "logged_out()");
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

static void metadata_updated( sp_session *session )
{
    TRACE_2( SPOTIFYMANAGER , "metadata_updated().");

    if( track != NULL )
    {
        printf("Track name: %s\n" , sp_track_name( track ) );

        playMusic( sp , track );

    }
}

static void notify_main_thread( sp_session *session )
{
    TRACE_2( SPOTIFYMANAGER , "notify_main_thread().");
}

static void log_message( sp_session *session , const char *data )
{
    TRACE_2( SPOTIFYMANAGER , "log_message().");
}

static void end_of_track( sp_session *session )
{
    TRACE_2( SPOTIFYMANAGER , "end_of_track().");

    printf("End of track...\n");

    audio_fifo_flush( &g_audiofifo );
    sp_session_player_play( session , 0 );
    sp_session_player_unload( session );

    playing = 0;
}

static int music_delivery( sp_session *session , const sp_audioformat *format , const void *frames , int num_frames )
{
    TRACE_2( SPOTIFYMANAGER , "music_delivery().");

    //printf("Playing music...%d\n" , num_frames );

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
    afd = malloc( sizeof( *afd ) + s );

    memcpy( afd->samples , frames , s );

    afd->nsamples = num_frames;
    afd->rate = format->sample_rate;
    afd->channels = format->channels;

    sendData( afd , s );

    TAILQ_INSERT_TAIL( &af->q , afd , link );
    af->qlen += num_frames;

    pthread_cond_signal( &af->cond );
    pthread_mutex_unlock( &af->mutex );

    return num_frames;
}
