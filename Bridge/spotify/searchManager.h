#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <libspotify/api.h>

#include "utils/types.h"
#include "utils/trace.h"
#include "network/serverManager.h"

void search( sp_session *session , char *query );

char *printAlbum( sp_album *album );
char *printArtist( sp_artist *artist );
char *printTrack( sp_track *track );
int trackDuration( sp_track *track );
char *trackUri( sp_track *track );


#endif // SEARCHMANAGER_H
