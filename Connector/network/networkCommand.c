#include "networkCommand.h"

static spotifytNetworkCommand_t spotifyNetworkCmd[] =
{
    {"CONFIG#DISPATCHER"    ,   &setDispatcherIp    },
    {"STREAMER#PLAY"        ,   &playStreamer       }
};


static int searchAction( char *command )
{
    TRACE_2( NETWORKCOMMAND , "searchAction( %s )." , command );

    int i = 0;

    for( i = 0 ; i < NETWORK_COUNT_COMMAND ; i++ )
    {
        if( strstr( command , spotifyNetworkCmd[i].command ) != NULL )
        {
            TRACE_1( NETWORKCOMMAND , "Command found, id: %d" , i );

            return i;
        }
    }

    return PC_ERROR;
}


void doAction( char *command )
{
    TRACE_2( NETWORKCOMMAND , "doAction( %s )." , command );

    int idFuncptr;

    char query[MAX_QUERY_LENGTH];
    char *arg = strrchr( command , '#');

    memset( query , 0 , MAX_QUERY_LENGTH );

    idFuncptr = searchAction( command );

    if( idFuncptr == PC_ERROR )
    {
        TRACE_ERROR( NETWORKCOMMAND , "Command is not supported.");

        return;
    }

    arg++; /* arg++ to not store the last '#' */

    snprintf( query , MAX_QUERY_LENGTH , "%s" , arg );

    TRACE_1( NETWORKCOMMAND , "Execute query : %s" , query );

    spotifyNetworkCmd[idFuncptr].func( query );
}
