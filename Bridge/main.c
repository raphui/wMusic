#include <stdio.h>
#include <stdlib.h>

#include "spotifyManager.h"
#include "types.h"

int main( void )
{

    if( launchSpotifyManager() == CONNECTION_ERROR )
    {
        printf("Connection to Spotify failed.\n");

        return PC_ERROR;
    }

    printf("\n");
    return 0;
}

