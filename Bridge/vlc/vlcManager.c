#include "vlcManager.h"

static libvlc_instance_t *vlcInstance = NULL;
static libvlc_media_t *vlcMedia = NULL;
static char *currentUrl = NULL;

static int initVlc( void )
{
    TRACE_2( VLCMANAGER , "initVlc()");

    int status = PC_SUCCESS;

    vlcInstance = libvlc_new( 0 , NULL );

    if( vlcInstance == NULL )
    {
        TRACE_ERROR( VLCMANAGER , "Fail to create a new vlc instance.");

        status = PC_ERROR;
    }

    currentUrl = ( char * )zmalloc( 124 * sizeof( char ) );

    return status;
}

int streamFile( const char *filename )
{
    TRACE_2( VLCMANAGER , "streamFile( %s )" , filename );

    int status = PC_SUCCESS;
    int ret;
    char output[50];

    memset( output , 0 , 50 );

    sprintf( output , "#http{dst=:1337/test3.wav}");

    if( vlcInstance == NULL )
        ret = initVlc();

    if( ret == PC_ERROR )
    {
        TRACE_ERROR( VLCMANAGER , "Fail to init VLC , cannot stream the file !");

        status = PC_ERROR;
    }
    else
    {
        vlcMedia = libvlc_media_new_path( vlcInstance , filename );

        if( vlcMedia != NULL )
        {

            ret = libvlc_vlm_add_broadcast( vlcInstance , "rtpStreaming" , filename , output , 0 , NULL , 1 , 0 );

            if( ret < 0 )
            {
                TRACE_ERROR( VLCMANAGER , "Cannot add a broadcast.");

                status = PC_ERROR;
            }
            else
            {
                TRACE_3( VLCMANAGER , "Broadcast stream added.");

                ret = libvlc_vlm_play_media( vlcInstance , "rtpStreaming");

                if( ret < 0 )
                {
                    TRACE_ERROR( VLCMANAGER , "Cannot diffuse the stream.");

                    status = PC_ERROR;
                }
                else
                {
                    TRACE_1( VLCMANAGER , "Start diffuse the stream.");
                }
            }
        }
        else
        {
            TRACE_ERROR( VLCMANAGER , "Fail to create a new media from the path: %s" , filename );

            status = PC_ERROR;
        }
    }

    return status;
}


int destroyVlc( void )
{
    TRACE_2( VLCMANAGER , "destroyVlc()");

    int status = PC_SUCCESS;

    if( vlcInstance != NULL )
        libvlc_release( vlcInstance );

    zfree( currentUrl );

    return status;
}

int loadStreamFromUrl( char *url , char *name )
{
    TRACE_2( VLCMANAGER , "loadStreamFromUrl( %s )." , url );

    int status = PC_SUCCESS;
    int ret;
    char output[50];

    memset( output , 0 , 50 );

    sprintf( output , "#http{dst=:1337/%s.wav}" , name );

    if( vlcInstance == NULL )
        ret = initVlc();

    if( ret == PC_ERROR )
    {
        TRACE_ERROR( VLCMANAGER , "Fail to init VLC , cannot stream the file !");

        status = PC_ERROR;
    }
    else
    {
        strncpy( currentUrl , url , strlen( url ) );

        registerNewStream( url , name );

        vlcMedia = libvlc_media_new_path( vlcInstance , url );

        if( vlcMedia != NULL )
        {

            ret = libvlc_vlm_add_broadcast( vlcInstance , name , url , output , 0 , NULL , 1 , 0 );

            if( ret < 0 )
            {
                TRACE_ERROR( VLCMANAGER , "Cannot add a broadcast.");

                status = PC_ERROR;
            }
            else
            {
                TRACE_3( VLCMANAGER , "Broadcast stream added.");

                ret = libvlc_vlm_play_media( vlcInstance , name );

                if( ret < 0 )
                {
                    TRACE_ERROR( VLCMANAGER , "Cannot diffuse the stream.");

                    status = PC_ERROR;
                }
                else
                {
                    TRACE_1( VLCMANAGER , "Start diffuse the stream.");
                }
            }
        }
        else
        {
            TRACE_ERROR( VLCMANAGER , "Fail to create a new media from the path: %s" , url );

            status = PC_ERROR;
        }
    }

    return status;
}

int playStream( const char *name )
{
    TRACE_2( VLCMANAGER , "playStream( %s )" , name );

    int status = PC_SUCCESS;
    int ret;

    ret = libvlc_vlm_play_media( vlcInstance , name );

    if( ret < 0 )
    {
        TRACE_ERROR( VLCMANAGER , "Cannot play the stream.");

        status = PC_ERROR;
    }
    else
    {
        TRACE_1( VLCMANAGER , "Play the stream.");
    }

    return status;
}

int pauseStream( const char *name )
{
    TRACE_2( VLCMANAGER , "pauseStream( %s )" , name );

    int status = PC_SUCCESS;
    int ret;

    ret = libvlc_vlm_pause_media( vlcInstance , name );

    if( ret < 0 )
    {
        TRACE_ERROR( VLCMANAGER , "Cannot pause the stream.");

        status = PC_ERROR;
    }
    else
    {
        TRACE_1( VLCMANAGER , "Pause the stream.");
    }

    return status;
}
