#include "playQueueManager.h"

static playqueue_fifo_t mainPlayqueue;
static playqueue_fifo_t *plfifo;

void initPlayqueue( void )
{
    TRACE_2( PLAYQUEUEMANAGER , "initPlayqueue()");

    plfifo = &mainPlayqueue;

    TAILQ_INIT( &plfifo->q );

    mainPlayqueue.qlen = 0;
}


int addTracksPlayqueue( sp_track *track )
{
    TRACE_2( PLAYQUEUEMANAGER , "addTracksPlayqueue()");

    playqueue_data_t *pldata;

    pldata = ( playqueue_data_t * )zmalloc( 1024 );

    pldata->track = track;

    TAILQ_INSERT_TAIL( &plfifo->q , pldata , link );

    TRACE_3( PLAYQUEUEMANAGER , "Track has been added.");

    return PC_SUCCESS;
}

sp_track *getNextTrack( void )
{
    TRACE_2( PLAYQUEUEMANAGER , "getNextTrack().");

    playqueue_data_t *pldata;
    sp_track *ret = NULL;

    pldata = TAILQ_FIRST( &plfifo->q );

    if( pldata == NULL )
    {
        TRACE_WARNING( PLAYQUEUEMANAGER , "Cannot get the next track, because the queue might be empty.");
    }
    else
    {
        TAILQ_REMOVE( &plfifo->q , pldata , link );

        TRACE_3( PLAYQUEUEMANAGER , "Track has been retrieve.");

        ret = pldata->track;
    }

    return ret;
}

int hasNextTrack( void )
{
    TRACE_2( PLAYQUEUEMANAGER , "hasNextTrack().");

    playqueue_data_t *pldata;

    pldata = TAILQ_FIRST( &plfifo->q );

    if( pldata != NULL )
        return TRUE;
    else
        return FALSE;
}
