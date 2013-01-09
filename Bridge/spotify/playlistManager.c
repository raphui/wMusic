#include "playlistManager.h"

static sp_playlistcontainer *plc;
static sp_session *currentSession;

static int countTrack = 0;
static int countMusicPlayed = 0;

void tracks_added( sp_playlist *pl , sp_track *const *tracks , int num_tracks , int position , void *userdata )
{
    TRACE_2( PLAYLISTMANAGER , "track_added()");

//    sp_playlist_add_tracks( pl , tracks , num_tracks , position , currentSession );

//    countTrack++;
}

void tracks_moved( sp_playlist *pl , const int *tracks , int num_tracks , int new_position , void *userdata )
{
    TRACE_2( PLAYLISTMANAGER , "tracks_moved()");

//    sp_playlist_reorder_tracks( pl , tracks , num_tracks , new_position );
}

void tracks_removed( sp_playlist *pl , const int *tracks , int num_tracks , void *userdata )
{
    TRACE_2( PLAYLISTMANAGER , "tracks_removed()");

//    sp_playlist_remove_tracks( pl , tracks , num_tracks );

//    countTrack--;
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
        TRACE_1( PLAYLISTMANAGER , "Success to retrieve the playlistcontainer");

        currentSession = session;
    }

    pthread_mutex_unlock( &mutexSession );

    return status;
}

int createPlaylist( const char *name )
{
    TRACE_2( PLAYLISTMANAGER , "createPlaylist( %s )" , name );

    int status = PC_SUCCESS;

    sp_playlist *pl = NULL;

    pthread_mutex_lock( &mutexSession );

    pl = sp_playlistcontainer_add_new_playlist( plc , name );

    if( pl == NULL )
    {
        TRACE_ERROR( PLAYLISTMANAGER , "Fail to create the new playlist : %s" , name );

        status = PC_ERROR;
    }
    else
    {
        TRACE_1( PLAYLISTMANAGER , "Success to create the new playlist : %s" , name );
    }

    pthread_mutex_unlock( &mutexSession );

    return status;
}

int removePlaylist( const char *name )
{
    TRACE_2( PLAYLISTMANAGER , "removePlaylist( %s )" , name );

    int status = PC_SUCCESS;
    int index;

    sp_error error;

    index = getPlaylistIndexByName( name );

    if( index == PC_ERROR )
    {
        TRACE_ERROR( PLAYLISTMANAGER , "Fail to retrieve playlist : %s." , name );

        status = PC_ERROR;
    }
    else
    {
        TRACE_3( PLAYLISTMANAGER , "Playlist have been got.");

        pthread_mutex_lock( &mutexSession );

        error = sp_playlistcontainer_remove_playlist( plc , index );

        pthread_mutex_unlock( &mutexSession );

        if( error != SP_ERROR_OK )
        {
            TRACE_ERROR( PLAYLISTMANAGER , "Cannot remove the playlist : %s." , name );

            status = PC_ERROR;
        }
        else
        {
            TRACE_1( PLAYLISTMANAGER , "Success to remove the playlist : %s." , name );
        }
    }

    return status;
}

int renamePlaylist( const char *name , const char *newName )
{
    TRACE_2( PLAYLISTMANAGER , "renamePlaylist( %s , %s )." , name , newName );

    int status = PC_SUCCESS;

    sp_playlist *pl = NULL;
    sp_error error;

    pl = getPlaylistByName( name );

    if( pl == NULL )
    {
        TRACE_ERROR( PLAYLISTMANAGER , "Fail to retrieve the playlist : %s." , name );

        status = PC_ERROR;
    }
    else
    {
        pthread_mutex_lock( &mutexSession );

        error = sp_playlist_rename( pl , newName );

        pthread_mutex_unlock( &mutexSession );

        if( error != SP_ERROR_OK )
        {
            TRACE_ERROR( PLAYLISTMANAGER , "Cannot rename the playlist : %s." , name );

            status = PC_ERROR;
        }
        else
        {
            TRACE_1( PLAYLISTMANAGER , "Success to rename the playlist : %s into %s." , name , newName );
        }
    }

    return status;
}

sp_playlist *getPlaylist( int index )
{
    TRACE_2( PLAYLISTMANAGER , "getPlaylist( %d )" , index );

    sp_playlist *pl;

    pthread_mutex_lock( &mutexSession );

    if( ( 0 < index ) && ( index < sp_playlistcontainer_num_playlists( plc ) ) )
    {
        TRACE_ERROR( PLAYLISTMANAGER , "The index : %d , it's not valid." , index );

        pl = NULL;
    }
    else
    {
        pl = sp_playlistcontainer_playlist( plc , index );

        TRACE_1( PLAYLISTMANAGER , "Playlist at index : %d , has been retrieved." , index );
    }

    pthread_mutex_unlock( &mutexSession );

    return pl;
}

int getPlaylistIndexByName( const char *name )
{
    TRACE_2( PLAYLISTMANAGER , "getPlaylistByName( %s )." , name );

    int i = 0;

    sp_playlist *pl = NULL;

    for( i = 0 ; i < sp_playlistcontainer_num_playlists( plc ) ; i++ )
    {
        pl = getPlaylist( i );

        if( strcmp( sp_playlist_name( pl ) , name ) == 0 )
        {
            TRACE_1( PLAYLISTMANAGER , "Playlist have been founded");

            return i;
        }
    }

    TRACE_3( PLAYLISTMANAGER , "Playlist have not been founded, return a NULL sp_playlist.");

    pl = NULL;

    return PC_ERROR;
}

sp_playlist *getPlaylistByName( const char *name )
{
    TRACE_2( PLAYLISTMANAGER , "getPlaylistByName( %s )." , name );

    int i = 0;

    sp_playlist *pl = NULL;

//    pthread_mutex_lock( &mutexSession );

    for( i = 0 ; i < sp_playlistcontainer_num_playlists( plc ) ; i++ )
    {
        pl = getPlaylist( i );

        if( strcmp( sp_playlist_name( pl ) , name ) == 0 )
        {
            TRACE_1( PLAYLISTMANAGER , "Playlist have been founded");

            return pl;
        }
    }

    TRACE_3( PLAYLISTMANAGER , "Playlist have not been founded, return a NULL sp_playlist.");

    pl = NULL;

//    pthread_mutex_unlock( &mutexSession );

    return pl;
}

int addTrackUriPlaylistByName( char *uri , const char *name )
{
    TRACE_2( PLAYLISTMANAGER , "addTrackUriPlaylistByName( %s , %s )." , uri , name );

    int status = PC_SUCCESS;
    int position = 0;
    int ret;

    sp_playlist *pl = NULL;
    sp_link *link = NULL;
    sp_track *track = NULL;

    pthread_mutex_lock( &mutexSession );

    link = sp_link_create_from_string( uri );

    pthread_mutex_unlock( &mutexSession );

    if( link == NULL )
    {
        TRACE_ERROR( PLAYLISTMANAGER , "Cannot get a link from the uri : %s." , uri );

        status = PC_ERROR;
    }
    else
    {
        pthread_mutex_lock( &mutexSession );

        track = sp_link_as_track( link );

        pthread_mutex_unlock( &mutexSession );

        if( track == NULL )
        {
            TRACE_ERROR( PLAYLISTMANAGER , "Cannot get a track from link.");

            status = PC_ERROR;
        }
        else
        {
            TRACE_3( PLAYLISTMANAGER , "Get the playlist.");

            pl = getPlaylistByName( name );

            if( pl == NULL )
            {
                TRACE_ERROR( PLAYLISTMANAGER , "Cannot get the playlist : %s." , name );

                status = PC_ERROR;
            }
            else
            {
                TRACE_3( PLAYLISTMANAGER , "Getting the position to insert the track.");

                position = sp_playlist_num_tracks( pl );


                TRACE_3( PLAYLISTMANAGER , "Adding track to the playlist : %s." , name );

                ret = addTrackPlaylistByName( track , name , position );

                if( ret == PC_ERROR )
                {
                    TRACE_3( PLAYLISTMANAGER , "Fail to add track to the playlist.");

                    status = PC_ERROR;
                }
                else
                {
                    TRACE_1( PLAYLISTMANAGER , "Track has been added.");
                }
            }

        }
    }

    return status;
}

int addTrackPlaylistByName( sp_track *track , const char *name , int position )
{
    TRACE_2( PLAYLISTMANAGER , "addTrackPlaylistByName( __track__ , %s , %d )." , name , position );

    int status = PC_SUCCESS;

    sp_error error;
    sp_playlist *pl = NULL;

    pl = getPlaylistByName( name );

    pthread_mutex_lock( &mutexSession );

    if( pl == NULL )
    {
        TRACE_ERROR( PLAYLISTMANAGER , "Cannot add track to the playlist, because playlist have not been founded.");

        status = PC_ERROR;
    }
    else
    {
        error = sp_playlist_add_tracks( pl , &track , 1 , position , currentSession );

        if( error != SP_ERROR_OK )
        {
            TRACE_ERROR( PLAYLISTMANAGER , "Cannot add track to the playlist, reason: %s" , sp_error_message( error ) );

            status = PC_ERROR;
        }
        else
        {
            TRACE_1( PLAYLISTMANAGER , "Track have been added !");
        }
    }

    pthread_mutex_unlock( &mutexSession );

    return status;
}

int addTrackPlaylist( sp_track *track , int index , int position )
{
    TRACE_2( PLAYLISTMANAGER , "addTrackPlaylist( __track__ , %d , %d )." , index , position );

    int status = PC_SUCCESS;

    sp_error error;
    sp_playlist *pl = NULL;

    pl = getPlaylist( index );

    pthread_mutex_lock( &mutexSession );

    if( pl == NULL )
    {
        TRACE_ERROR( PLAYLISTMANAGER , "Cannot add track to the playlist, because playlist have not been founded.");

        status = PC_ERROR;
    }
    else
    {
        error = sp_playlist_add_tracks( pl , &track , 1 , position , currentSession );

        if( error != SP_ERROR_OK )
        {
            TRACE_ERROR( PLAYLISTMANAGER , "Cannot add track to the playlist, reason: %s" , sp_error_message( error ) );

            status = PC_ERROR;
        }
        else
        {
            TRACE_1( PLAYLISTMANAGER , "Track have been added !");
        }
    }

    pthread_mutex_unlock( &mutexSession );

    return status;
}


sp_playlist *getStarredList( sp_session *session )
{
    TRACE_2( PLAYLISTMANAGER , "getStarredList().");

    sp_playlist *pl= NULL;

    pthread_mutex_lock( &mutexSession );

    pl = sp_session_starred_create( session );

    if( pl == NULL )
    {
        TRACE_ERROR( PLAYLISTMANAGER , "Cannot get the starred playlist ( maybe no user id logged in ? ).");
    }
    else
    {
        TRACE_1( PLAYLISTMANAGER , "Starred playlist have been retrieved.");
    }

    pthread_mutex_unlock( &mutexSession );

    return pl;
}

int setStarredTrack( sp_session *session , sp_track *track , int starred )
{
    TRACE_2( PLAYLISTMANAGER , "setStarredTrack().");

    int status = PC_SUCCESS;
    sp_error error;

    pthread_mutex_lock( &mutexSession );

    error = sp_track_set_starred( session , &track , 1 , starred );

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( PLAYLISTMANAGER , "Cannot change the starred status of the track.");

        status = PC_ERROR;
    }
    else
    {
        TRACE_1( PLAYLISTMANAGER , "The starred status of track have been changed.");
    }

    pthread_mutex_unlock( &mutexSession );

    return status;
}
