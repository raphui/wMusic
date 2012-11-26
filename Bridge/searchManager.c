#include "searchManager.h"

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
        printf("Query:      %s\n" , sp_search_query( search ) );

        for( i = 0 ; i < sp_search_num_artists( search ) ; i++ )
        {
            printArtist( sp_search_artist( search , i ) );
        }
    }

    sp_search_release( search );
}

void printTrack( sp_album *album )
{

}

void printArtist( sp_artist *artist )
{
    printf("Artist:         %s\n" , sp_artist_name( artist ) );
}
