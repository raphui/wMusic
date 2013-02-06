#ifndef VLCMANAGER_H
#define VLCMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <vlc/vlc.h>

#include "utils/trace.h"
#include "utils/types.h"
#include "utils/zmemory.h"
#include "network/streamManager.h"

int playUrl( char *url );
void destroyVlc( void );

#endif // VLCMANAGER_H
