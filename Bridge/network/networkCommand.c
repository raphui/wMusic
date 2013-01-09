#include "networkCommand.h"

static spotifytNetworkCommand_t spotifyNetworkCmd[] =
{

    {"PLAYER#LOAD"       ,   &loadMusic      ,   NULL           },
    {"PLAYER#PLAY"       ,   &playMusic      ,   NULL           },
    {"PLAYER#PAUSE"      ,   &pauseMusic     ,   NULL           },
    {"SEARCH#BASIC"      ,   &search         ,   NULL           },
    {"SEARCH#ARTIST"     ,   &search         ,   "artist:"      },
    {"SEARCH#ALBUM"      ,   &search         ,   "album:"       },
    {"SEARCH#TRACK"      ,   &search         ,   "track:"       },
    {"SEARCH#WHATSNEW"   ,   &search         ,   "tag:new"      }
};


static int searchAction( char *command , char *arg2 )
{
    TRACE_2( NETWORKCOMMAND , "searchAction( %s )." , command );

    int i = 0;

    for( i = 0 ; i < NETWORK_COUNT_COMMAND ; i++ )
    {
        if( strstr( command , spotifyNetworkCmd[i].command ) != NULL )
        {
            TRACE_1( NETWORKCOMMAND , "Command found, id: %d" , i );

            if( spotifyNetworkCmd[i].specificArg != NULL )
                memcpy( arg2 , spotifyNetworkCmd[i].specificArg , strlen( spotifyNetworkCmd[i].specificArg ) );

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

    char *arg = strrchr( command , '#' );
    char arg2[255];

    memset( query , 0 , MAX_QUERY_LENGTH );
    memset( arg2 , 0 , 255 );

    idFuncptr = searchAction( command , arg2 );

    if( idFuncptr == PC_ERROR )
    {
        TRACE_ERROR( NETWORKCOMMAND , "Command is not supported.");

        return;
    }

    if( arg2[0] != 0 )
    {
        strcat( query , arg2 );
        strcat( query , arg + 1 );
    }
    else
    {
        strcat( query , arg + 1 );
    }

    TRACE_1( NETWORKCOMMAND , "Execute query : %s" , query );

    spotifyNetworkCmd[idFuncptr].executeCommand( g_session , query );
}
