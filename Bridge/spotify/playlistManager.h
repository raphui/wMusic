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

#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libspotify/api.h>

#include "utils/types.h"
#include "utils/trace.h"

void tracks_added( sp_playlist *pl , sp_track *const *tracks , int num_tracks , int position , void *userdata );
void tracks_moved( sp_playlist *pl , const int *tracks , int num_tracks , int new_position , void *userdata );
void tracks_removed( sp_playlist *pl , const int *tracks , int num_tracks , void *userdata );
void playlist_metadata_updated( sp_playlist *pl , void *userdata );

int initPlaylistManager( sp_session *session );
int reloadPlaylistContainer( void );
int getPlaylistContainer( sp_session *session );
int createPlaylist( const char *name );
int removePlaylist( const char *name );
int renamePlaylist( const char *name , const char *newName );
sp_playlist *getPlaylist( int index );
int getPlaylistIndexByName( const char *name );
sp_playlist *getPlaylistByName( const char *name );
int addTrackUriPlaylistByName( char *uri , const char *name );
int addTrackPlaylistByName( sp_track *track , const char *name , int position );
int addTrackPlaylist( sp_track *track , int index , int position );
sp_playlist *getStarredList( sp_session *session );
int setStarredTrack( sp_session *session , sp_track *track , int starred );

#endif // PLAYLISTMANAGER_H
