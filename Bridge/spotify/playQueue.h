#ifndef PLAYQUEUE_H
#define PLAYQUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/queue.h>
#include <libspotify/api.h>

#include "utils/types.h"
#include "utils/trace.h"


typedef struct playqueue_data
{
    TAILQ_ENTRY( playqueue_data ) link;
    sp_track *track;

}playqueue_data_t;


typedef struct playqueue_fifo
{
    TAILQ_HEAD( , playqueue_data ) q;
    int qlen;

}playqueue_fifo_t;


#endif // PLAYQUEUE_H
