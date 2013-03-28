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

#ifndef STREAMMANAGER_H
#define STREAMMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spotify/playQueueManager.h"
#include "spotify/playerManager.h"
#include "spotify/playlistManager.h"
#include "utils/types.h"
#include "utils/trace.h"
#include "utils/zmemory.h"
#include "vlc/vlcManager.h"
#include "network/serverManager.h"

#define MAX_STREAM  10

typedef struct streamProperties
{
    char *url;
    char *name;
    playqueue_fifo_t *playqueue;

}streamProperties_t;

streamProperties_t streamProps[MAX_STREAM];

int initStreamer( void );
int loadStreamer( char *url , char *name );
int playStreamer( char *name );
int pauseStreamer( char *name );
int nextTrackInStream( char *name );
int loadPlaylistInStream( char *playlist , char *name );
int registerNewStream( char *url , char *name );
int unregisterStream( char *name );
int getStreamUrlFromName( const char *name );
void getInfosCurrentTrack( char *name );

#endif // STREAMMANAGER_H
