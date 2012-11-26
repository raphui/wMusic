#include <stdio.h>
#include <stdlib.h>

#include "spotifyManager.h"
#include "types.h"

int main( void )
{

    if( launchSpotifyManager() == CONNECTION_ERROR )
    {
        return PC_ERROR;
    }

    printf("\n");
    return 0;
}

