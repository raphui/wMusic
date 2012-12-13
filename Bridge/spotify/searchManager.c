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
    TRACE_2( SEARCHMANAGER , "searchComplete()");

    int i = 0;
    int trackDur;
    int searchCount;
    int sizeToBeSend;

    char duration[32];
    char start[] = "<results>";
    char stop[] = "</results>";
    char *toBeSend;

    sp_track *currentTrack;
    sp_artist *currentArtist;
    sp_album *currentAlbum;

//    pthread_mutex_lock( &mutexSession );

    if( sp_search_error( search ) == SP_ERROR_OK )
    {
        TRACE_3( SEARCHMANAGER , "Query: %s" , sp_search_query( search ) );

        searchCount = sp_search_num_tracks( search );

        sizeToBeSend = 512 * searchCount;

        toBeSend = ( char * )malloc( sizeToBeSend );

        memset( toBeSend , 0 , sizeToBeSend );

        strcat( toBeSend , start );

        for( i = 0 ; i <  searchCount ; i++ )
        {
            currentTrack = sp_search_track( search , i );
            currentArtist = sp_search_artist( search , i );
            currentAlbum = sp_search_album( search , i );

            if( ( currentTrack == NULL )
                    || ( currentArtist == NULL )
                    || ( currentAlbum == NULL ) )
            {
                TRACE_WARNING( SEARCHMANAGER , "currentTrack or currentArtist or currentAlbum is NULL.");

                continue;
            }

            trackDur = trackDuration( currentTrack );

            sprintf( duration , "%d" , trackDur );

            strcat( toBeSend , "<result><uri>");

            strcat( toBeSend , trackUri( currentTrack ) );
            strcat( toBeSend , "</uri><track>");
            strcat( toBeSend , printTrack( currentTrack ) );
            strcat( toBeSend , "</track><artist>");
            strcat( toBeSend , printArtist( currentArtist ) );
            strcat( toBeSend , "</artist><time>");
            strcat( toBeSend , duration );
            strcat( toBeSend , "</time><album>");
            strcat( toBeSend , printAlbum( currentAlbum ) );
            strcat( toBeSend , "</album>");

            strcat( toBeSend , "</result>");

        }

        strcat( toBeSend , stop );

        sendVoid( toBeSend , sizeToBeSend );

        free( toBeSend );
    }

    sp_search_release( search );

//    pthread_mutex_unlock( &mutexSession );
}

char *printAlbum( sp_album *album )
{
    TRACE_2( SEARCHMANAGER , "printAlbum()");

//    pthread_mutex_lock( &mutexSession );

    TRACE_3( SEARCHMANAGER , "Album:\t\t %s" , sp_album_name( album ) );

//    pthread_mutex_unlock( &mutexSession );

    return sp_album_name( album );
}

char *printArtist( sp_artist *artist )
{
    TRACE_2( SEARCHMANAGER , "printArtist()");

//    pthread_mutex_lock( &mutexSession );

    TRACE_3( SEARCHMANAGER , "Artist:\t\t %s" , sp_artist_name( artist ) );

//    pthread_mutex_unlock( &mutexSession );

    return sp_artist_name( artist );
}

char *printTrack( sp_track *track )
{
    TRACE_2( SEARCHMANAGER , "printTrack()");

//    pthread_mutex_lock( &mutexSession );

    TRACE_3( SEARCHMANAGER , "Track:\t\t %s" , sp_track_name( track ) );

//    pthread_mutex_unlock( &mutexSession );

    return sp_track_name( track );
}

int trackDuration( sp_track *track )
{
    TRACE_2( SEARCHMANAGER , "trackDuration()");

    return sp_track_duration( track );
}

char *trackUri( sp_track *track )
{
    TRACE_2( SEARCHMANAGER , "trackUri()");

    char uri[255];

    memset( uri , 0 , 255 );

    sp_link_as_string( sp_link_create_from_track( track , 0 ) , uri , 255 );

    return uri;
}
