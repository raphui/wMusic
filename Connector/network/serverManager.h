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

#include "utils/types.h"
#include "utils/trace.h"
#include "utils/threadManager.h"
#include "network/networkCommand.h"
#include "system/cli.h"
#include "vlc/vlcManager.h"

#define BUFF_SIZE       1024
#define MAX_CLIENT      10
#define PORT_STREAMER   1337
#define PORT_COMMANDER  1338
#define PORT_CLI        1339

typedef struct argumentReceivingThread
{
    int socket;
    int port;

}argumentReceivingThread_t;

void launchServer( void );
void createServer( void *port );
int closeServer( void );
void receivingThread( void *arg );
int disconnectClient( int *socket );

void sendVoid( void *data , size_t size );
void sendVoidSocket( int socket , void *data , size_t size );

pthread_mutex_t mutex;

#endif // SERVER_H

