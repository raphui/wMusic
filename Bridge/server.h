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
#include "networkCommand.h"

#define BUFF_SIZE       1024
#define MAX_CLIENT      10
#define PORT_STREAMER   1337
#define PORT_COMMANDER  1338
#define MULTICAST_ADDR  "224.2.2.2"
#define MULTICAST_PORT  1337


int initMulticastSocket();

void launchServer( void );
void createServer();
int closeServer( void );

void receivingThread( void *socket );

int disconnectClient( int *socket );

void sendData( audio_fifo_data_t *data , size_t size );
void sendControl( char *command );
void sendVoid( void *data , size_t size );
void sendDataMulticast( audio_fifo_data_t *data , size_t size );
void sendControlMulticast( char *command );
void sendVoidMulticast( void *data , size_t size );

pthread_mutex_t mutex;

#endif // SERVER_H
