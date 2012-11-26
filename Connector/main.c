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

//#define IP_SERVER   "127.0.0.1"
#define IP_SERVER   "192.168.1.11"
#define BUFF_SIZE   4
#define DATA_SIZE   8192

static audio_fifo_t g_audiofifo;

static int countPackets = 0;

int play( audio_fifo_data_t *data )
{
//    printf("Playing music...%d\n" , data->nsamples );

    audio_fifo_t *af = &g_audiofifo;
    audio_fifo_data_t *afd;

    afd = ( audio_fifo_data_t * ) malloc( DATA_SIZE + sizeof( *afd ) );

    memcpy( afd , data , DATA_SIZE );

    if( afd->nsamples == 0 )
    {
        pthread_mutex_unlock( &af->mutex );
        return 0;
    }

    if( af->qlen > afd->rate )
        return 0;

    TAILQ_INSERT_TAIL( &af->q , afd , link );
    af->qlen += afd->nsamples;

    pthread_cond_signal( &af->cond );
    pthread_mutex_unlock( &af->mutex );

    return afd->nsamples;
}

int main( void )
{

    int sock;
    struct sockaddr_in serverAddr;

    char message[] = "PLAYER:PLAY:zae";

    audio_fifo_data_t *buff;
    size_t size = DATA_SIZE + sizeof( *buff );
    ssize_t b;

    audio_init( &g_audiofifo );

    buff = ( audio_fifo_data_t * )malloc( DATA_SIZE + sizeof( *buff ) );

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

//    send( sock , message , sizeof( message ) , 0 );

    while( 1 )
    {
        b = read( sock , buff , size );

        if( b > 0 )
        {

            countPackets++;

            printf("######### %d packets received ! ######\n\t\t SIZE:%d\tBYTES:%d\n" , countPackets , DATA_SIZE + sizeof( *buff ) , b );

            printf("Playing music...%d\n" , buff->nsamples );

            play( buff );

            memset( buff , 0 , DATA_SIZE );

        }
        else
        {
            printf("Cannot receive data.\n");
        }
    }

    printf("\n");
    return 0;
}

