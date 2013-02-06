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

const char *dispatcherIp;

void setDispatcherIp( char *ip );
int playStreamer( char *url );

#endif // STREAMMANAGER_H
