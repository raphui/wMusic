#include "threadManager.h"

void createThread( void *routine , void *arg )
{
    TRACE_2( THREADMANAGER , "createThread()");

    pthread_t newThread;

    if( arg == NULL )
    {
        pthread_create( &newThread , NULL , ( void * ) &routine , NULL );
    }
    else
    {
        pthread_create( &newThread , NULL , ( void * ) &routine , arg );
    }
}
