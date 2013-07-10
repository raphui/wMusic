/*  wMusic - Music system software
 *  Copyright (C) 2013  Raphaël POGGI
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "serverManager.h"

static int lastRequester;
static int portCommander = PORT_COMMANDER;
static int portCli = PORT_CLI;

//static pthread_t serverStreamerThread;
static pthread_t serverCommanderThread;
static pthread_t serverCliThread;

void launchServer( void )
{
    TRACE_2( SERVERMANAGER , "lanchServer()");

    TRACE_1( SERVERMANAGER , "Start server on port %d..." , portCommander );

    pthread_create( &serverCommanderThread , NULL , ( void * )&createServer , &portCommander );

    TRACE_1( SERVERMANAGER , "Start server on port %d..." , portCli );

    pthread_create( &serverCliThread , NULL , ( void * )&createServer , &portCli );

    /* We have to increment the thread count by 2, because we don't use createThread() function. */
    incrementThreadCount( 2 );

}


void createServer( void *port )
{
    TRACE_2( SERVERMANAGER , "createServer( %d )." , *( int * )port );

    int s_server = socket( AF_INET , SOCK_STREAM , 0 );
    int clients[MAX_CLIENT];
    int numClients;

    struct sockaddr_in serv_addr;
    argumentReceivingThread_t *args;

    if( s_server < 0 )
    {
        TRACE_ERROR( SERVERMANAGER , "[-]Error to create socket.");

        pthread_exit( ( void * )PC_ERROR );
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl( INADDR_ANY );
    serv_addr.sin_port = htons( *( int * )port );

    if( bind( s_server , ( struct sockaddr* )&serv_addr, sizeof( serv_addr ) ) < 0 )
    {
        TRACE_ERROR( SERVERMANAGER , "[-]Error to bind on port: %d." , *( int * )port );

        pthread_exit( ( void * )PC_ERROR );
    }

    if( listen( s_server , 10 ) < 0 )
    {
        TRACE_ERROR( SERVERMANAGER , "[-]Error to listen to 10 connection.");

        pthread_exit( ( void * )PC_ERROR );
    }

    while( 1 )
    {
        if( numClients < MAX_CLIENT )
        {
            clients[numClients] = accept( s_server , NULL , NULL );

            if( clients[numClients] > 0 )
            {

                TRACE_1( SERVERMANAGER , "[!]New client connected.");

                args = ( argumentReceivingThread_t * )zmalloc( sizeof( argumentReceivingThread_t ) );

                args->socket = clients[numClients];
                args->port = *( int * )port;

                createThread( &receivingThread , args );

                numClients++;
            }
        }
    }

    releaseThread();

    pthread_exit( ( void * )PC_SUCCESS );
}


int closeServer( void )
{
    TRACE_2( SERVERMANAGER , "close().");

    return PC_SUCCESS;
}


/* void *arg is a argumentReceivingThread_t type */
void receivingThread( void *arg )
{
    TRACE_2( SERVERMANAGER , "receivingThread()");

    char buff[BUFF_SIZE];
    char input[] = "wMusic~>";
    int ret;
    int info;
    void *cliRet;

    argumentReceivingThread_t *arguments = ( argumentReceivingThread_t * )arg;

    TRACE_3( SERVERMANAGER , "Thread argument: socket: %d , port: %d" , arguments->socket , arguments->port );

    TRACE_1( SERVERMANAGER , "[!]Receiving thread create !");

    if( arguments->port == PORT_CLI )
        sendVoidSocket( arguments->socket , input , sizeof( input ) );
    else //usleep( 2000 ) for WAN connection, usleep( 300 ) for LAN connection.
        usleep( 2000 ); info = IAMBRIDGE; sendVoidSocket( arguments->socket , ( void *)&info , sizeof( int ) );    //Sending info that the remote is connected to a bridge.

    while( 1 )
    {
        memset( buff , 0 , BUFF_SIZE );

        ret = recv( arguments->socket , buff , BUFF_SIZE , 0 );

        if( ret > 0 )
        {

            if( arguments->port == PORT_COMMANDER )
            {
                TRACE_3( SERVERMANAGER , "[+]Data: %s" , buff );

                if( strstr( buff , "DISC") != NULL )
                {
                    disconnectClient( &arguments->socket );

                    break;
                }

                lastRequester = arguments->socket;

                doAction( buff );
            }
            else if( arguments->port == PORT_CLI )
            {
                TRACE_3( SERVERMANAGER , "[+]Data: %s" , buff );

                cliRet = doCommand( buff );

                if( cliRet != NULL )
                {
                    sendVoidSocket( arguments->socket , cliRet , strlen( ( const char * )cliRet ) );

                    zfree( cliRet );

                    sendVoidSocket( arguments->socket , input , sizeof( input ) );
                }
            }
        }

    }

    TRACE_3( SERVERMANAGER , "[!]Quitting receiving thread !");

    releaseThread();

    pthread_exit( NULL );
}

int disconnectClient( int *socket )
{
    TRACE_2( SERVERMANAGER , "disconnectClient()");

    int status = PC_SUCCESS;


    if( close( *socket ) < 0 )
    {
        TRACE_ERROR( SERVERMANAGER , "Cannot close client socket.");

        status = PC_ERROR;
    }
    else
    {
        TRACE_1( SERVERMANAGER , "Client disconnect.");
    }

    return status;
}

void sendVoid( void *data , size_t size )
{
    TRACE_2( SERVERMANAGER , "sendVoid()");

    ssize_t b = 0;

    if( lastRequester != 0 )
    {
        b = send( lastRequester , data , size , 0 );

        if( b < 0 )
            TRACE_WARNING( SERVERMANAGER , "Fail to send data");
    }

}

void sendVoidSocket( int socket , void *data , size_t size )
{
    TRACE_2( SERVERMANAGER , "sendVoidSocket()");

    ssize_t b = 0;

    if( socket != 0 )
    {
        b = send( socket , data , size , 0 );

        if( b < 0 )
            TRACE_WARNING( SERVERMANAGER , "Fail to send data");
    }
    else
    {
        TRACE_ERROR( SERVERMANAGER , "Cannot send data, socket might be disconnected.");
    }
}
