#include "spotifyManager.h"

int launchSpotifyManager( void )
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

    error = sp_session_create( &spconfig , &g_session );

    if( error != SP_ERROR_OK )
    {
        printf("Fail to create session, reason: %s\n" , sp_error_message( error ) );
    }
    else
    {
        printf("Success to create session\n");
    }

    if( g_session != NULL )
    {
        if( signin( g_session , USERNAME , PASSWORD ) == CONNECTION_OK )
        {
            launchServer();

            running = 1;
            playing = 0;

            while( running )
            {
                pthread_mutex_lock( &mutexSession );

                sp_session_process_events( g_session , &next_timeout );

                pthread_mutex_unlock( &mutexSession );

                if( ( login == 1 ) && ( playing == 0 ) )
                {
                    printf("Let's play the music !\n");


                    playing = 1;
                }
            }
        }
        else
        {
            return CONNECTION_ERROR;
        }
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
