#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libspotify/api.h>
#include "audio.h"

#define USERNAME "othane"
#define PASSWORD "TestRaphio"


sp_session *sp;
sp_track *track;
static audio_fifo_t g_audiofifo;

int playing;
int running;
int login = 0;

static void logged_in( sp_session *session , sp_error error)
{

    if( error != SP_ERROR_OK )
    {
        printf("Fail to login, reason: %s.\n" ,  sp_error_message( error ) );

        exit( 1 );
    }

    printf("Success to login.\n");

    login = 1;
}

static void logged_out( sp_session *session )
{

}

static void metadata_updated( sp_session *session )
{
    sp_error error;

    if( track != NULL )
    {
        printf("Track name: %s" , sp_track_name( track ) );
    }

    error = sp_session_player_load( sp , track );

    if( error != SP_ERROR_OK )
    {
        printf("Cannot load track, reason: %s" , sp_error_message( error ) );
    }
}

static void connection_error( sp_session *session , sp_error error )
{

}

static void notify_main_thread( sp_session *session )
{

}

static void log_message( sp_session *session , const char *data )
{

}

static void offline_status_updated( sp_session *session )
{

}

static void credentials_blob_updated( sp_session *session , const char *blob )
{

}

static void end_of_track( sp_session *session )
{

    audio_fifo_flush( &g_audiofifo );
    sp_session_player_play( session , 0 );
    sp_session_player_unload( session );

    playing = 0;
}

static void start_playback( sp_session *session )
{

}

static void stop_playback( sp_session *session )
{

}

static int music_delivery( sp_session *session , const sp_audioformat *format , const void *frames , int num_frames )
{
    printf("Playing music...\n");

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

    TAILQ_INSERT_TAIL( &af->q , afd , link );
    af->qlen += num_frames;

    pthread_cond_signal( &af->cond );
    pthread_mutex_unlock( &af->mutex );

    return num_frames;
}

void play( sp_session *sp , sp_track *track )
{
    sp_error error;

    error = sp_session_player_load( sp , track );

    if( error != SP_ERROR_OK )
    {
        printf("Cannot load track, reason: %s" , sp_error_message( error ) );
    }
    else
    {
        printf("Track loaded.\n");
    }
}

const uint8_t g_appkey[] = {
 0x01, 0x88, 0x00, 0x53, 0x92, 0x9E, 0x65, 0x93, 0xF8, 0x24, 0x44, 0xAF, 0x6B, 0x00, 0x8F, 0xEF,
 0xDB, 0xBE, 0x70, 0x66, 0xD3, 0x36, 0xBA, 0x16, 0x00, 0x62, 0xB2, 0xBE, 0x4D, 0xD6, 0x21, 0xEF,
 0x1F, 0xB3, 0xC7, 0xDB, 0x0D, 0xA6, 0xE6, 0xB0, 0xF8, 0x60, 0xA3, 0xC5, 0x2F, 0x71, 0x5D, 0xD4,
 0xF7, 0x74, 0xE8, 0x53, 0x02, 0xD0, 0x63, 0x67, 0x1B, 0x4E, 0x89, 0xA8, 0x65, 0x83, 0x95, 0xAA,
 0x04, 0x32, 0x8E, 0x40, 0xBA, 0xF7, 0x0D, 0xA0, 0x1F, 0x3F, 0xA2, 0x5C, 0xDD, 0xC6, 0x55, 0x6A,
 0xAF, 0x66, 0x86, 0xD0, 0x62, 0x8A, 0x95, 0xE1, 0x2E, 0x97, 0xBF, 0xFA, 0xD7, 0xC6, 0xE8, 0x34,
 0xF1, 0x13, 0x2B, 0x0D, 0xDD, 0xDF, 0x69, 0xDA, 0x68, 0xEF, 0xB4, 0x55, 0x3C, 0x26, 0x8C, 0x3D,
 0x8F, 0x77, 0x0A, 0x94, 0x38, 0xAD, 0x40, 0x1E, 0xF4, 0xE1, 0x2F, 0xF1, 0xFE, 0xFD, 0xA9, 0x90,
 0xD5, 0x0C, 0x33, 0x51, 0x0F, 0x22, 0x76, 0x51, 0x0F, 0x3C, 0x0A, 0x90, 0xC9, 0xFB, 0x76, 0x3F,
 0x28, 0x2E, 0x75, 0xA7, 0x20, 0xD3, 0x65, 0xB9, 0x0F, 0x69, 0x74, 0xAC, 0xCE, 0x39, 0x20, 0xA9,
 0xA5, 0x65, 0xEA, 0x17, 0x5B, 0xA5, 0x98, 0x92, 0x68, 0x55, 0xF7, 0x63, 0x66, 0x4D, 0xB6, 0xB7,
 0x77, 0x45, 0x30, 0x93, 0xEB, 0x3B, 0x11, 0x10, 0x97, 0x06, 0x87, 0x58, 0x08, 0xE3, 0xCD, 0x11,
 0x5E, 0x7C, 0xBF, 0xB4, 0x52, 0x78, 0x1E, 0xFD, 0xDE, 0xD7, 0xE1, 0x9B, 0xF4, 0x45, 0x33, 0xDA,
 0x20, 0x55, 0xE0, 0x10, 0x13, 0x3F, 0x02, 0x9F, 0x76, 0x5E, 0x9D, 0x3B, 0x30, 0xB1, 0xED, 0xD9,
 0x15, 0x97, 0x76, 0xC6, 0x93, 0x17, 0x1D, 0xD9, 0x4B, 0x2B, 0xFE, 0x63, 0xD1, 0x01, 0xE6, 0x25,
 0xB8, 0x1A, 0x83, 0x12, 0xC2, 0x9A, 0xCF, 0xF2, 0x54, 0xEE, 0x99, 0x5F, 0x7B, 0xE6, 0xFA, 0xA5,
 0xE2, 0x83, 0xD9, 0xA7, 0xFE, 0xDC, 0xBF, 0xB4, 0x6F, 0x2B, 0xB4, 0x03, 0x8A, 0xCA, 0x3D, 0xED,
 0x23, 0x97, 0xC1, 0xF2, 0xDB, 0x5A, 0xA1, 0x83, 0x9C, 0x03, 0xC4, 0xAD, 0x5F, 0x93, 0xBD, 0xF3,
 0xAC, 0x9B, 0x7E, 0xCA, 0xCC, 0x1A, 0xD3, 0x6E, 0x64, 0x83, 0x40, 0xD7, 0x24, 0x7A, 0x32, 0xF3,
 0xD6, 0x5D, 0xDF, 0x39, 0x40, 0x1F, 0xA8, 0x92, 0x12, 0xB4, 0x12, 0xEF, 0x9F, 0x27, 0x0C, 0xFE,
 0xE6,
};

const size_t g_appkey_size = sizeof( g_appkey );

static sp_session_callbacks spSessionCallbacks;

int main( void )
{
    sp_session_config spconfig;
    sp_error error;
    sp_link *link;

    char username[] = USERNAME;
    char password[] = PASSWORD;

    int next_timeout = 0;

    memset( &spconfig , 0 , sizeof( spconfig ) );
    memset( &spSessionCallbacks , 0 , sizeof( spSessionCallbacks ) );

    spSessionCallbacks.logged_in = &logged_in;
    spSessionCallbacks.notify_main_thread = &notify_main_thread;
    spSessionCallbacks.end_of_track = &end_of_track;
    spSessionCallbacks.start_playback = &start_playback;
    spSessionCallbacks.stop_playback = &stop_playback;
    spSessionCallbacks.music_delivery = &music_delivery;
//    spSessionCallbacks.logged_out = &logged_out;
//    spSessionCallbacks.metadata_updated = &metadata_updated;
//    spSessionCallbacks.connection_error = &connection_error;

//    spSessionCallbacks.log_message = &log_message;
//    spSessionCallbacks.offline_status_updated = &offline_status_updated;
//    spSessionCallbacks.credentials_blob_updated = &credentials_blob_updated;

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

        printf("Creating URI...\n");

        link = sp_link_create_from_string("spotify:track:56fwHfJaBpaauvFJrnwk2L");

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
            printf("Faibooll to create track.\n");

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

//        error = sp_session_player_load( sp , track );
//        error = sp_session_player_play( sp , 1 );

//        if( error != SP_ERROR_OK )
//        {
//            printf("Cannot load track, reason: %s" , sp_error_message( error ) );
//        }

        running = 1;
        playing = 0;

        while( running )
        {
            sp_session_process_events( sp , &next_timeout );

            if( ( login == 1 ) && ( playing == 0 ) )
            {
                play( sp , track );

                playing = 1;
            }
        }

    }


    printf("\n");
    return 0;
}

