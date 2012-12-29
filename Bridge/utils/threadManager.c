#include "threadManager.h"

static int countThreads = 0;

void createThread( void *routine , void *arg )
{
    TRACE_2( THREADMANAGER , "createThread()");

    pthread_t newThread;

    if( arg == NULL )
    {
        pthread_create( &newThread , NULL , ( void * ) routine , NULL );

        countThreads++;
    }
    else
    {
        pthread_create( &newThread , NULL , ( void * ) routine , arg );

        countThreads++;
    }

    TRACE_1( THREADMANAGER , "Current number of threads: %d." , countThreads );
}

void incrementThreadCount( int n )
{
    TRACE_2( THREADMANAGER , "incrementThreadCount( %d )." , n );

    countThreads += n;
}

char *getThreadCount( void )
{
    TRACE_2( THREADMANAGER , "getThreadCount().");

    char *buff = ( char * )zmalloc( 40 * sizeof( char ) );

    memset( buff , 0 , 40 );

    sprintf( buff , "Total thread running : %d\n" , countThreads );

    return buff;
}

void releaseThread( void )
{
    TRACE_2( THREADMANAGER , "releaseThread().");

    countThreads--;
}
