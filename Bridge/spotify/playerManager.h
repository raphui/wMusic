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


int play( sp_session *session , char *uri );

int createTrackFromUri( char *uri , char *name );
int loadMusic( sp_session *session , char *uri , char *name );
int playMusic( sp_session *session , char *uri , char *name );
int pauseMusic( sp_session *session , char *uri , char *name  );
int nextMusic( sp_session *session , char *uri );

int running;
int playing;
int pausing;


#endif // PLAYERMANAGER_H
