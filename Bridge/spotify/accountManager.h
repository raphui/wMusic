#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libspotify/api.h>

#include "utils/types.h"
#include "utils/trace.h"
#include "spotify/playlistManager.h"
#include "network/serverManager.h"

/* ############# CALLBACK functions ############# */
void logged_in( sp_session *session , sp_error error);
void logged_out( sp_session *session );

void initAccountManager( sp_session *session );

int login( const char *username , const char *password );
int logout( sp_session *session );

int logged;

#endif // ACCOUNTMANAGER_H
