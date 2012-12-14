#include "playlistManager.h"

static sp_playlistcontainer *plc;
static sp_playlist *mainPlaylist;

void tracks_added( sp_playlist *pl , sp_track *const *tracks , int num_tracks , int position , void *userdata )
{
    TRACE_2( PLAYLISTMANAGER , "track_added()");
}

void tracks_moved( sp_playlist *pl , const int *tracks , int num_tracks , int new_position , void *userdata )
{
    TRACE_2( PLAYLISTMANAGER , "tracks_moved()");
}

void tracks_removed( sp_playlist *pl , const int *tracks , int num_tracks , void *userdata )
{
    TRACE_2( PLAYLISTMANAGER , "tracks_removed()");
}

void playlist_metadata_updated( sp_playlist *pl , void *userdata )
{
    TRACE_2( PLAYLISTMANAGER , "playlist_metadata_updated()");
}

int getPlaylistContainer( sp_session *session )
{
    TRACE_2( PLAYLISTMANAGER , "getPlaylistContainer()");

    int status = PC_SUCCESS;

    pthread_mutex_lock( &mutexSession );

    plc = sp_session_playlistcontainer( session );

    if( plc == NULL )
    {
        TRACE_ERROR( PLAYLISTMANAGER , "Fail to retrieve the playlistcontainer.");

        status = PC_ERROR;
    }
    else
    {
        TRACE_3( PLAYLISTMANAGER , "Success to retrieve the playlistcontainer");
    }

    pthread_mutex_unlock( &mutexSession );

    return status;
}

int createPlaylist( const char *name )
{
    TRACE_2( PLAYLISTMANAGER , "createPlaylist( %s )" , name );

    int status = PC_SUCCESS;

    pthread_mutex_lock( &mutexSession );

    mainPlaylist = sp_playlistcontainer_add_new_playlist( plc , name );

    if( mainPlaylist == NULL )
    {
        TRACE_ERROR( PLAYLISTMANAGER , "Fail to create the new playlist : %s" , name );

        status = PC_ERROR;
    }
    else
    {
        TRACE_3( PLAYLISTMANAGER , "Success to create the new playlist : %s" , name );
    }

    pthread_mutex_unlock( &mutexSession );

    return status;
}

int addTracksMainPlaylist( sp_session *session , sp_track *track )
{
    TRACE_2( PLAYLISTMANAGER , "addTracksMainPlaylist()");

    int status = PC_SUCCESS;

    sp_error error;

    pthread_mutex_lock( &mutexSession );

    error = sp_playlist_add_tracks( mainPlaylist , track , 1 , 2 , session );

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( PLAYLISTMANAGER , "Fail to add track to the main playlist.");

        status = PC_ERROR;
    }
    else
    {
        TRACE_3( PLAYLISTMANAGER , "Success to add track to the main playlist.");
    }

    pthread_mutex_unlock( &mutexSession );

    return status;
}
