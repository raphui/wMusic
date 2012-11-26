#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libspotify/api.h>

#include "types.h"
#include "trace.h"

/* ############# CALLBACK functions ############# */
static void searchComplete( sp_search *search , void *userdata );

void search( sp_session *session , char *query );
void printTrack( sp_album *album );
void printArtist( sp_artist *artist );

#endif // SEARCHMANAGER_H
