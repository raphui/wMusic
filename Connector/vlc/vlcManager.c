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
