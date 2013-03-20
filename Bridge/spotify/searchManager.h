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

#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <libspotify/api.h>

#include "utils/types.h"
#include "utils/trace.h"
#include "utils/zmemory.h"
#include "network/serverManager.h"

#define MAX_SEARCH_COUNT    100

int search( sp_session *session , char *query );

const char *printAlbum( sp_album *album );
const char *printArtist( sp_artist *artist );
const char *printTrack( sp_track *track );
int trackDuration( sp_track *track );
char *trackUri( sp_track *track , char *uri );


#endif // SEARCHMANAGER_H
