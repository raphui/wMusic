#include "server.h"

static int s_client[MAX_CLIENT];
static int countClients;

static pthread_t serverStreamerThread;
static pthread_t serverCommanderThread;

void launchServer( void )
{
    TRACE_2( SPOTIFYMANAGER , "lanchServer()");

    int portStreamer = 1337;
    int portCommander = 1338;

    printf("Start server on port %d...\n" , portStreamer );

    pthread_create( &serverStreamerThread , NULL , ( void * )&createServer , portStreamer );

    printf("Start server on port %d...\n" , portCommander );

    pthread_create( &serverCommanderThread , NULL , ( void * )&createServer , portCommander );
}

void createServer( int port )
{
    TRACE_2( STREAMINGSERVER , "createServer( %d )." , port );

    int s_server = socket( AF_INET , SOCK_STREAM , 0 );

    struct sockaddr_in serv_addr;

    if( s_server < 0 )
    {
        printf("[-]Error to create socket.\n");

        pthread_exit( PC_ERROR );
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl( INADDR_ANY );
    serv_addr.sin_port = htons( port );

    if( bind( s_server , ( struct sockaddr* )&serv_addr, sizeof( serv_addr ) ) < 0 )
    {
        printf("[-]Error to bind on port: %d.\n" , port );

        pthread_exit( PC_ERROR );
    }

    if( listen( s_server , 10 ) < 0 )
    {
        printf("[-]Error to listen to 10 connection.\n");

        pthread_exit( PC_ERROR );
    }

    while( 1 )
    {
        if( countClients < MAX_CLIENT )
        {
            s_client[countClients] = accept( s_server , NULL , NULL );

            printf("[!]New client connected.\n");


            //Only port commander, accept to receive commands.
            if( port == PORT_COMMANDER )
                createThread( &receivingThread , &s_client[countClients] );

            countClients++;
        }
    }


    pthread_exit( PC_SUCCESS );
}


int closeServer( void )
{
    TRACE_2( STREAMINGSERVER , "close().");

    return PC_SUCCESS;
}

void receivingThread( void *socket )
{
    char buff[BUFF_SIZE];
    char *arg;
    int ret;

    printf("[!]Receiving thread create !\n");

//    play( g_session , arg );

    while( 1 )
    {
        memset( buff , 0 , BUFF_SIZE );

        ret = recv( *( int *)socket , buff , BUFF_SIZE , 0 );

        if( ret > 0 )
        {
            printf("[+]Data: %s\n" , buff );

            if( strstr( buff , "PLAYER:PLAY" ) != NULL )
            {
                arg = strstr( buff , "spotify" );

                //g_session from spotifyManager.h
                play( g_session , arg );
            }
            else if( strstr( buff , "SEARCH:ARTIST") != NULL )
            {
                arg = strstr( buff , "David" );

                //g_session from spotifyManager.h
                search( g_session , arg );
            }
        }

    }

    printf("[!]Quitting receiving thread !\n");

    pthread_exit( NULL );
}


void sendData( audio_fifo_data_t *data , size_t size )
{
    TRACE_2( STREAMINGSERVER , "sendData().");

    static int countPackets = 0;
    static FILE *f;

    f = fopen("/home/raphio/serv.txt" , "a" );

    ssize_t b;

    if( s_client[countClients - 1] != 0 )
    {
        b = write( s_client[countClients - 1] , data , size );

//        b = send( s_client[countClients - 1] , data , size , 0 );

        if( b < 0 )
        {
            printf("Cannot write data to client.\n");
        }
        else
        {
//            printf("Playing music...%d\n" , data->nsamples );

            fprintf( f, "Playing music...\n");
            fprintf( f, "Channels:\t %d\n" , data->channels );
            fprintf( f, "Rate:\t\t %d\n" , data->rate );
            fprintf( f, "NSamples:\t %d\n" , data->nsamples );
            fprintf( f, "Samples:\t %d\n" , data->samples[0] );

            fclose( f );

            countPackets++;

            printf("######### %d packets sended ! ###### \t\t SIZE:%d\tBYTES:%d\n" , countPackets , size , b );
        }
    }

}

void sendControl( char *command )
{
    if( s_client[countClients - 1] != 0 )
    {
        send( s_client[countClients - 1] , command , 6 , 0 );
    }
}


void sendVoid( void *data , size_t size )
{
    size_t b = 0;
    size_t tmpSize = size / 8;

    if( s_client[countClients - 1] != 0 )
    {

//        if( size == 8192 )
//        {
//            while( b != size )
//                b += send( s_client[countClients - 1] , data + b , tmpSize , 0 );
//        }
//        else
//        {
            send( s_client[countClients - 1] , data , size , 0 );

//        }
    }

}
