#ifndef VLCMANAGER_H
#define VLCMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <vlc/vlc.h>

#include "utils/trace.h"
#include "utils/types.h"
#include "utils/zmemory.h"

int streamFile( const char *filename );
int destroyVlc( void );
int loadStreamFromUrl( const char *url );
int playStream( const char *name );
int pauseStream( const char *name );

#endif // VLCMANAGER_H
