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

#include <stdio.h>
#include <stdlib.h>

#include "spotify/spotifyManager.h"
#include "system/environmentManager.h"
#include "utils/types.h"

int main( void )
{

#if SET_ENV
    TRACE_INFO( SPOTIFYMANAGER , "Setting up system environment.");

    setAdhoc( ESSID , KEY );
#endif

    TRACE_INFO( SPOTIFYMANAGER , "Starting Spotify manager.");

    if( launchSpotifyManager() == CONNECTION_ERROR )
    {
        TRACE_ERROR( SPOTIFYMANAGER , "Connection to Spotify failed.");

        return PC_ERROR;
    }

    printf("\n");
    return 0;
}

