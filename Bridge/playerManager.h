#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libspotify/api.h>

#include "audio.h"
#include "types.h"
#include "trace.h"
#include "server.h"

/* ############# CALLBACK functions ############# */
void metadata_updated( sp_session *session );
int music_delivery( sp_session *session , const sp_audioformat *format , const void *frames , int num_frames );
void end_of_track( sp_session *session );


int play( sp_session *session , char *uri );

static void playMusic( sp_session *sp , sp_track *track );

static sp_track *track;
static audio_fifo_t g_audiofifo;

static int running;
static int playing;

#endif // PLAYERMANAGER_H
