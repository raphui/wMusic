#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/trace.h"
#include "utils/types.h"
#include "vlc/vlcManager.h"

#if CROSS_COMPILE
#define REF_FILE    "/home/pi/music.wav"
#define STREAM_FILE "/home/pi/test3.wav"
#else
#define REF_FILE    "/home/raphio/music.wav"
#define STREAM_FILE "/home/raphio/test3.wav"
#endif

#define DATA_SIZE   8192


int createFile( void );
int writeFile( void *data );

#endif // FILEMANAGER_H
