#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <libspotify/api.h>

#include "utils/types.h"
#include "utils/trace.h"

void search( sp_session *session , char *query );

void printAlbum( sp_album *album );
void printArtist( sp_artist *artist );
void printTrack( sp_track *track );


#endif // SEARCHMANAGER_H
