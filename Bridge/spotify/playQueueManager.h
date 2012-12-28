#ifndef PLAYQUEUEMANAGER_H
#define PLAYQUEUEMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/queue.h>
#include <libspotify/api.h>

#include "utils/types.h"
#include "utils/trace.h"
#include "utils/zmemory.h"
#include "spotify/playQueue.h"


void initPlayqueue( void );
int addTracksPlayqueue( sp_track *track );
sp_track *getNextTrack( void );
int hasNextTrack( void );

#endif // PLAYQUEUEMANAGER_H
