#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "network/serverManager.h"

int running = 1;

int main( void )
{

    launchServer();

    TRACE_INFO( SERVERMANAGER , "Ready to be used." );

    while( running );

    printf("\n");
    return 0;
}

