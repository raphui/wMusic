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

#define BUFF_SIZE   1024

static int s_client;

void createServer( int port );
int closeServer( void );
void receivingThread( void *socket );
void sendData( audio_fifo_data_t *data , size_t size );
//void sendData( int sample_rate , int channels , int frames , int num_frames );

#endif // SERVER_H
