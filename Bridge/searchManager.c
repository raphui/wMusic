#include "searchManager.h"

static void searchComplete( sp_search *search , void *userdata );

void search( sp_session *session , char *query )
{
    TRACE_2( SEARCHMANAGER , "search( __session__ , %s )." , query );

    pthread_mutex_lock( &mutexSession );

    sp_search_create( session , query , 0 , 100 , 0 , 100 , 0 , 100 , 0 , 100 , SP_SEARCH_STANDARD , &searchComplete , NULL );

    pthread_mutex_unlock( &mutexSession );
}

static void searchComplete( sp_search *search , void *userdata )
{
    int i = 0;

    pthread_mutex_lock( &mutexSession );

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

    pthread_mutex_unlock( &mutexSession );
}

void printAlbum( sp_album *album )
{
    pthread_mutex_lock( &mutexSession );

    TRACE_3( SEARCHMANAGER , "Album:\t\t %s" , sp_album_name( album ) );

    pthread_mutex_unlock( &mutexSession );
}

void printTrack( sp_track *track )
{
    pthread_mutex_lock( &mutexSession );

    TRACE_3( SEARCHMANAGER , "Track:\t\t %s" , sp_track_name( track ) );

    pthread_mutex_unlock( &mutexSession );
}

void printArtist( sp_artist *artist )
{
    pthread_mutex_lock( &mutexSession );

    TRACE_3( SEARCHMANAGER , "Artist:\t\t %s" , sp_artist_name( artist ) );

    pthread_mutex_unlock( &mutexSession );
}
