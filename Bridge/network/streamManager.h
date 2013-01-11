#ifndef STREAMMANAGER_H
#define STREAMMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/types.h"
#include "utils/trace.h"
#include "utils/zmemory.h"
#include "vlc/vlcManager.h"
#include "network/serverManager.h"
#include "network/multicastAddrManager.h"

int loadStreamer( const char *url );
int playStreamer( const char *addr );
int pauseStreamer( const char *addr );
int getMulticastAddr( const char *url );

#endif // STREAMMANAGER_H
