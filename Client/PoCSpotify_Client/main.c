#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IP_SERVER "127.0.0.1"

int main( void )
{

    int sock;
    struct sockaddr_in serverAddr;


    if( sock = socket( AF_INET , SOCK_STREAM , 0 ) < 0 )
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

    printf("\n");
    return 0;
}

