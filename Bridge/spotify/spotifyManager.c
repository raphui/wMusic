#include "spotifyManager.h"

static sp_session_callbacks spSessionCallbacks;


static void notify_main_thread( sp_session *session );
static void log_message( sp_session *session , const char *data );
static int initPlaylist( void );

int launchSpotifyManager( void )
{
    TRACE_2( SPOTIFYMANAGER , "launchSpotifyManager()");

    sp_session_config spconfig;
    sp_error error;

    int next_timeout = 0;

    memset( &spconfig , 0 , sizeof( spconfig ) );
    memset( &spSessionCallbacks , 0 , sizeof( spSessionCallbacks ) );

    spSessionCallbacks.logged_in = &logged_in;
    spSessionCallbacks.notify_main_thread = &notify_main_thread;
    spSessionCallbacks.end_of_track = &end_of_track;
    spSessionCallbacks.music_delivery = &music_delivery;
    spSessionCallbacks.metadata_updated = &metadata_updated;

    spconfig.api_version = SPOTIFY_API_VERSION;
    spconfig.cache_location = CACHE_LOCATION;
    spconfig.settings_location = SETTINGS_LOCATION;
    spconfig.application_key = g_appkey;
    spconfig.application_key_size = g_appkey_size;
    spconfig.user_agent = SPOTIFY_USER_AGENT;
    spconfig.callbacks = &spSessionCallbacks;

    error = sp_session_create( &spconfig , &g_session );

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( SPOTIFYMANAGER , "Fail to create session, reason: %s" , sp_error_message( error ) );
    }
    else
    {
        TRACE_1( SPOTIFYMANAGER , "Success to create session");

        TRACE_3( SPOTIFYMANAGER , "Set preferred bitrate to : %d." , PREFERRED_BITRATE );

        error = sp_session_preferred_bitrate( g_session , PREFERRED_BITRATE );

        if( error != SP_ERROR_OK )
        {
            TRACE_WARNING( SPOTIFYMANAGER , "Fail to set preferred bitrate to %d." , PREFERRED_BITRATE );
        }
    }

    if( g_session != NULL )
    {
        if( signin( g_session , USERNAME , PASSWORD ) == CONNECTION_OK )
        {
            launchServer();

            running = TRUE;
            playing = FALSE;

            TRACE_INFO( SPOTIFYMANAGER , "Ready to be used !");

            while( running )
            {
                pthread_mutex_lock( &mutexSession );

                sp_session_process_events( g_session , &next_timeout );

                pthread_mutex_unlock( &mutexSession );

            }
        }
        else
        {
            return CONNECTION_ERROR;
        }
    }

    return CONNECTION_OK;
}

static void notify_main_thread( sp_session *session )
{
    TRACE_2( SPOTIFYMANAGER , "notify_main_thread().");
}

static void log_message( sp_session *session , const char *data )
{
    TRACE_2( SPOTIFYMANAGER , "log_message().");
}
