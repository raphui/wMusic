#ifndef VLCMANAGER_H
#define VLCMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <vlc/vlc.h>

#include "trace.h"
#include "types.h"

int initVlc( void );
int streamFile( const char *filename );
int destroyVlc( void );

#endif // VLCMANAGER_H
