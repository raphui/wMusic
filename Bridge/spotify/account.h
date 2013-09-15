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

#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libspotify/api.h>

#include "utils/types.h"
#include "utils/trace.h"
#include "spotify/playlist.h"
#include "network/server.h"

/* ############# CALLBACK functions ############# */
void logged_in( sp_session *session , sp_error error);
void logged_out( sp_session *session );

void initAccountManager( sp_session *session );

int login( const char *username , const char *password );
int logout( sp_session *session );

int logged;

#endif // ACCOUNTMANAGER_H
