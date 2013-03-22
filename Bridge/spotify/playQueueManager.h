/*  wMusic - Music system software
 *  Copyright (C) 2013  Raphaël POGGI
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

#define MAX_PLAYQUEUE   10

void initPlayqueue( void );

int addTracksToPlayqueue( playqueue_fifo_t *plfifo , sp_track *track );
sp_track *getNextTrackToPlayqueue( playqueue_fifo_t *playqueue );
int hasNextTrackFromPlayqueue( playqueue_fifo_t *playqueue );

int addTracksPlayqueue( sp_track *track );
sp_track *getNextTrack( void );
int hasNextTrack( void );
char *dumpPlayQueue( void );

playqueue_fifo_t *getPlayqueue( void );

#endif // PLAYQUEUEMANAGER_H
