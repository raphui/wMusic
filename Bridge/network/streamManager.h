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

#define MAX_STREAM  10

typedef struct streamProperties
{
    char *url;
    char *name;

}streamProperties_t;

streamProperties_t streamProps[MAX_STREAM];

int initStreamer( void );
int loadStreamer( char *url , char *name );
int playStreamer( const char *name );
int pauseStreamer( const char *name );
int registerNewStream( char *url , char *name );
int unregisterStream( char *name );
int getStreamUrlFromName( const char *name );

#endif // STREAMMANAGER_H
