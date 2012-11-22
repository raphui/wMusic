#include "server.h"

int createServer( void )
{
    int s_server = socket( AF_INET , SOCK_STREAM , 0 );
    int s_client;

    struct sockaddr_in serv_addr;

    pthread_t thread;

    if( s_server < 0 )
    {
        printf("[-]Error to create socket.\n");

        return PC_ERROR;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl( INADDR_ANY );
    serv_addr.sin_port = htons( 1337 );

    if( bind( s_server , ( struct sockaddr* )&serv_addr, sizeof( serv_addr ) ) < 0 )
    {
        printf("[-]Error to bind on port: 1337.\n");

        return PC_ERROR;
    }

    if( listen( s_server , 10 ) < 0 )
    {
        printf("[-]Error to listen to 10 connection.\n");

        return PC_ERROR;
    }

    while( 1 )
    {

        s_client = accept( s_server , NULL , NULL );

        printf("[!]New client connected.\n");
        pthread_create( &thread , NULL , receivingThread , &s_client );

    }


    return PC_SUCCESS;
}


int closeServer( void )
{

    return PC_SUCCESS;
}

void receivingThread( void *socket )
{
    char buff[BUFF_SIZE];
    int ret;

    printf("[!]Receiving thread create !\n");

    while( 1 )
    {
        memset( buff , 0 , BUFF_SIZE );

        ret = recv( *( int *)socket , buff , BUFF_SIZE , 0 );

        if( ret > 0 )
        {
            printf("[+]Data: %s\n" , buff );

        }

    }

    printf("[!]Quitting receiving thread !\n");

    pthread_exit( NULL );
}
