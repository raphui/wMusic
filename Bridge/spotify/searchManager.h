#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <libspotify/api.h>

#include "utils/types.h"
#include "utils/trace.h"
#include "utils/zmalloc.h"
#include "network/serverManager.h"

#define MAX_SEARCH_COUNT    100

void search( sp_session *session , char *query );

const char *printAlbum( sp_album *album );
const char *printArtist( sp_artist *artist );
const char *printTrack( sp_track *track );
int trackDuration( sp_track *track );
char *trackUri( sp_track *track , char *uri );


#endif // SEARCHMANAGER_H
