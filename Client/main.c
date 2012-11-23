#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "audio.h"

#define IP_SERVER   "127.0.0.1"
#define BUFF_SIZE   4
#define DATA_SIZE   8192

static audio_fifo_t g_audiofifo;

int sock;
//int buff[BUFF_SIZE];

struct sockaddr_in serverAddr;

typedef struct data{

    int sample_rate;
    int channels;
    int16_t frames;
    int num_frames;

}s_data;

//int play( int sample_rate , int channels , int frames , int num_frames )
int play( audio_fifo_data_t *data )
{

    printf("Playing music...%d\n" , data->nsamples );

    audio_fifo_t *af = &g_audiofifo;
    audio_fifo_data_t *afd;
    size_t s;

//    afd = data;

    afd = ( audio_fifo_data_t * ) malloc( DATA_SIZE * sizeof( audio_fifo_data_t ) );

    memcpy( afd , data , DATA_SIZE );

    // audio discontinuity, do nothing
//    if( num_frames == 0 )
//    {
//        pthread_mutex_unlock( &af->mutex );
//        return 0;
//    }

    if( afd->nsamples == 0 )
    {
        pthread_mutex_unlock( &af->mutex );
        return 0;
    }

//    // buffer one second of audio
//    if( af->qlen > sample_rate )
//        return 0;

    if( af->qlen > afd->rate )
        return 0;

//    s = num_frames * sizeof( int16_t ) * channels;
//    afd = malloc( sizeof( *afd ) + s );

//    memcpy( afd->samples , frames , s );

//    afd->nsamples = num_frames;
//    afd->rate = sample_rate;
//    afd->channels = channels;

    TAILQ_INSERT_TAIL( &af->q , afd , link );
//    af->qlen += num_frames;
    af->qlen += afd->nsamples;

    pthread_cond_signal( &af->cond );
    pthread_mutex_unlock( &af->mutex );

//    return num_frames;
    return afd->nsamples;
}

int main( void )
{

    audio_fifo_data_t *buff;

    audio_init( &g_audiofifo );

    buff = ( audio_fifo_data_t * ) malloc( DATA_SIZE * sizeof( audio_fifo_data_t ) );

    sock = socket( AF_INET , SOCK_STREAM , 0 );

    if( sock < 0 )
    {
        printf("Cannot create socket...\n");

        return -1;
    }

    printf("Socket has been created.\n");

    serverAddr.sin_addr.s_addr = inet_addr( IP_SERVER );
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons( 1337 );

    if( connect( sock , ( struct sockaddr * )&serverAddr , sizeof( struct sockaddr ) ) < 0 )
    {
        printf("Cannot connect to the server...\n");

        return -1;
    }

    while( 1 )
    {

        memset( buff , 0 , DATA_SIZE );

        if( read( sock , buff , DATA_SIZE ) > 0 )
        {

            printf("Playing music...%d\n" , buff->nsamples );

            play( buff );
        }
        else
        {
            printf("Cannot receive data.\n");
        }
    }

    printf("\n");
    return 0;
}

