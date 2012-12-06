#include "searchManager.h"

static void searchComplete( sp_search *search , void *userdata );

void search( sp_session *session , char *query )
{
    pthread_mutex_lock( &mutexSession );

    sp_search_create( session , query , 0 , 100 , 0 , 100 , 0 , 100 , 0 , 100 , SP_SEARCH_STANDARD , &searchComplete , NULL );

    pthread_mutex_unlock( &mutexSession );
}

static void searchComplete( sp_search *search , void *userdata )
{
    int i = 0;

    if( sp_search_error( search ) == SP_ERROR_OK )
    {
        printf("Query:\t\t %s\n" , sp_search_query( search ) );

        for( i = 0 ; i < sp_search_num_artists( search ) ; i++ )
        {
            printArtist( sp_search_artist( search , i ) );
        }
    }

    sp_search_release( search );
}

void printAlbum( sp_album *album )
{
    printf("Album:\t\t %s\n" , sp_album_name( album ) );
}

void printTrack( sp_track *track )
{
    printf("Track:\t\t %s\n" , sp_track_name( track ) );
}

void printArtist( sp_artist *artist )
{
    printf("Artist:\t\t %s\n" , sp_artist_name( artist ) );
}
