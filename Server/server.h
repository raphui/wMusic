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

#define BUFF_SIZE   1024

typedef struct data{

    int sample_rate;
    int channels;
    int16_t frames;
    int num_frames;

}s_data;

static int s_client;

void createServer( void );
int closeServer( void );
void receivingThread( void *socket );
void sendData( audio_fifo_data_t *data , size_t size );
//void sendData( int sample_rate , int channels , int frames , int num_frames );

#endif // SERVER_H
