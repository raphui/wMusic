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
