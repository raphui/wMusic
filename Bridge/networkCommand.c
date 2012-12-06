#include "networkCommand.h"

static networkCommand_t networkCmd[] = {

    {   "PLAYER#PLAY"       ,   &play               },
    {   "PLAYER#PAUSE"      ,   &pauseMusic         },
    {   "SEARCH#ARTIST"     ,   &search             },
    {   "SEARCH#ALBUM"      ,   &search             },
    {   "SEARCH#TRACK"      ,   &search             }
};


static int searchAction( char *command )
{
    TRACE_2( NETWORKCOMMAND , "searchAction( %s )." , command );

    int i = 0;

    for( i = 0 ; i < COUNT_COMMAND ; i++ )
    {
        if( strstr( command , networkCmd[i].command ) != NULL )
        {
            TRACE_1( NETWORKCOMMAND , "Command found, id: %d" , i );

            return networkCmd[i].executeCommand;
        }
    }
}


void doAction( char *command )
{
    TRACE_2( NETWORKCOMMAND , "doAction( %s )." , command );

    char *arg = strrchr( command , '#' );

    int ( *execute )( sp_session* , char* ) = searchAction( command );

    execute( g_session , arg + 1 );
}
