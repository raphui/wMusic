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

