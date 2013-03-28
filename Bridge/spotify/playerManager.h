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

#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libspotify/api.h>

#include "audio/audio.h"
#include "utils/types.h"
#include "utils/trace.h"
#include "utils/zmemory.h"
#include "network/serverManager.h"
#include "vlc/vlcManager.h"
#include "file/fileManager.h"
#include "spotify/playlistManager.h"
#include "spotify/playQueueManager.h"

/* ############# CALLBACK functions ############# */
void metadata_updated( sp_session *session );
int music_delivery( sp_session *session , const sp_audioformat *format , const void *frames , int num_frames );
void end_of_track( sp_session *session );

void initPlayerEnv( void );
int play( sp_session *session , char *uri );

int createTrackFromUri( char *uri , char *name );
int loadMusic( sp_session *session , char *uri , char *name , playqueue_fifo_t *playqueue );
int playMusic( sp_session *session , char *uri , char *name , playqueue_fifo_t *playqueue );
int pauseMusic( sp_session *session , char *uri , char *name  );
int nextMusic( sp_session *session , char *uri );
char *getTrackInfos( void );

int running;
int playing;
int pausing;


#endif // PLAYERMANAGER_H
