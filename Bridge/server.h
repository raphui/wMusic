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

#include "audio.h"
#include "types.h"
#include "trace.h"
#include "threadManager.h"
#include "spotifyManager.h"

#define BUFF_SIZE       1024
#define MAX_CLIENT      10
#define PORT_STREAMER   1337
#define PORT_COMMANDER  1338

static int s_client[MAX_CLIENT];
static int countClients;

void launchServer( void );
void createServer( int port );
int closeServer( void );
void acceptingThread( void *s_server );
void receivingThread( void *socket );
void sendData( audio_fifo_data_t *data , size_t size );
void sendControl( char *command );
void sendVoid( int16_t data , size_t size );

static pthread_t serverStreamerThread;
static pthread_t serverCommanderThread;

#endif // SERVER_H
