#include "searchManager.h"

static void searchComplete( sp_search *search , void *userdata );

void search( sp_session *session , char *query )
{
    TRACE_2( SEARCHMANAGER , "search( __session__ , %s )." ,query );

    pthread_mutex_lock( &mutexSession );

    sp_search_create( session , query , 0 , 100 , 0 , 100 , 0 , 100 , 0 , 100 , SP_SEARCH_STANDARD , &searchComplete , NULL );

    pthread_mutex_unlock( &mutexSession );
}

static void searchComplete( sp_search *search , void *userdata )
{
    int i = 0;

    if( sp_search_error( search ) == SP_ERROR_OK )
    {
        TRACE_3( SEARCHMANAGER , "Query: %s" , sp_search_query( search ) );

        for( i = 0 ; i < sp_search_num_tracks( search ) ; i++ )
        {
            printTrack( sp_search_track( search , i ) );
        }

        for( i = 0 ; i < sp_search_num_albums( search ) ; i++ )
        {
            printAlbum( sp_search_album( search , i ) );
        }

        for( i = 0 ; i < sp_search_num_artists( search ) ; i++ )
        {
            printArtist( sp_search_artist( search , i ) );
        }
    }

    sp_search_release( search );
}

void printAlbum( sp_album *album )
{
    TRACE_3( SEARCHMANAGER , "Album: %s" , sp_album_name( album ) );
}

void printTrack( sp_track *track )
{
    TRACE_3( SEARCHMANAGER , "Track: %s" , sp_track_name( track ) );
}

void printArtist( sp_artist *artist )
{
    TRACE_3( SEARCHMANAGER , "Artist: %s" , sp_artist_name( artist ) );
}
