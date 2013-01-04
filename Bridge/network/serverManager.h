#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <unistd.h>

#include "audio/audio.h"
#include "utils/types.h"
#include "utils/trace.h"
#include "utils/threadManager.h"
#include "spotify/spotifyManager.h"
#include "network/networkCommand.h"
#include "network/multicastAddrManager.h"
#include "system/cli.h"
#include "vlc/vlcManager.h"

#define BUFF_SIZE       1024
#define MAX_CLIENT      10
#define PORT_STREAMER   1337
#define PORT_COMMANDER  1338
#define PORT_CLI        1339
#define MULTICAST_ADDR  "224.2.2.2"
#define MULTICAST_PORT  1337

typedef struct argumentReceivingThread
{
    int socket;
    int port;

}argumentReceivingThread_t;

int initMulticastSocket( void );
void launchServer( void );
void createServer( void *port );
int closeServer( void );
void receivingThread( void *arg );
int disconnectClient( int *socket );

void sendVoid( void *data , size_t size );
void sendVoidSocket( int socket , void *data , size_t size );

pthread_mutex_t mutex;

#endif // SERVER_H

