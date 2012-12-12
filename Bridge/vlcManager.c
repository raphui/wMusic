#include "vlcManager.h"

static libvlc_instance_t *vlcInstance;
static libvlc_media_t *vlcMedia;
static libvlc_media_player_t *vlcMediaPlayer;

int initVlc( void )
{
    TRACE_2( VLCMANAGER , "initVlc");

    int status = PC_SUCCESS;

    vlcInstance = libvlc_new( 0 , NULL );

    if( vlcInstance == NULL )
    {
        TRACE_ERROR( VLCMANAGER , "Fail to create a new vlc instance.");

        status = PC_ERROR;
    }

    return status;
}

int streamFile( const char *filename )
{
    TRACE_2( VLCMANAGER , "streamFile( %s )" , filename );

    int status = PC_SUCCESS;

    vlcMedia = libvlc_media_new_path( vlcInstance , filename );

    if( vlcMedia != NULL )
    {

    }
    else
    {
        TRACE_ERROR( VLCMANAGER , "Fail to create a new media from the path: %s" , filename );
    }

    return status;
}


int destroyVlc( void )
{
    TRACE_2( VLCMANAGER , "destroyVlc()");

    int status = PC_SUCCESS;

    if( vlcInstance != NULL )
        libvlc_release( vlcInstance );

    return status;
}
