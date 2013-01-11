#include "networkCommand.h"

static spotifytNetworkCommand_t spotifyNetworkCmd[] =
{
    {"ACCOUNT#LOGIN"        ,   NULL                        ,   &login          ,   NULL            ,   0   },
    {"ACCOUNT#LOGOUT"       ,   &logout                     ,   NULL            ,   NULL            ,   1   },
    {"PLAYQUEUE#LOAD"       ,   NULL                        ,   &loadMusic      ,   NULL            ,   1   },
    {"PLAYQUEUE#PLAY"       ,   NULL                        ,   &playMusic      ,   NULL            ,   1   },
    {"PLAYQUEUE#PAUSE"      ,   NULL                        ,   &pauseMusic     ,   NULL            ,   1   },
    {"PLAYQUEUE#NEXT"       ,   NULL                        ,   &nextMusic      ,   NULL            ,   1   },
    {"PLAYLIST#ADD"         ,   &createPlaylist             ,   NULL            ,   NULL            ,   0   },
    {"PLAYLIST#REMOVE"      ,   &removePlaylist             ,   NULL            ,   NULL            ,   0   },
    {"PLAYLIST#RENAME"      ,   NULL                        ,   &renamePlaylist ,   NULL            ,   0   },
    {"PLAYLIST#ADDTRACK"    ,   &addTrackUriPlaylistByName  ,   NULL            ,   NULL            ,   0   },
    {"PLAYLIST#REMOVETRACK" ,   NULL                        ,   NULL            ,   NULL            ,   0   },  /* Not implemented !*/
    {"SEARCH#BASIC"         ,   NULL                        ,   &search         ,   NULL            ,   1   },
    {"SEARCH#ARTIST"        ,   NULL                        ,   &search         ,   "artist:"       ,   1   },
    {"SEARCH#ALBUM"         ,   NULL                        ,   &search         ,   "album:"        ,   1   },
    {"SEARCH#TRACK"         ,   NULL                        ,   &search         ,   "track:"        ,   1   },
    {"SEARCH#WHATSNEW"      ,   NULL                        ,   &search         ,   "tag:new"       ,   1   },
    {"STREAMER#LOAD"        ,   &loadStreamer               ,   NULL            ,   NULL            ,   0   },  /* Not implemented !*/
    {"STREAMER#PLAY"        ,   &playStreamer               ,   NULL            ,   NULL            ,   0   },  /* Not implemented !*/
    {"STREAMER#PAUSE"       ,   &pauseStreamer              ,   NULL            ,   NULL            ,   0   },  /* Not implemented !*/
    {"STREAMER#GETADDR"     ,   &getMulticastAddr           ,   NULL            ,   NULL            ,   0   }   /* Not implemented !*/
};


static int searchAction( char *command , char *specificArg )
{
    TRACE_2( NETWORKCOMMAND , "searchAction( %s )." , command );

    int i = 0;

    for( i = 0 ; i < NETWORK_COUNT_COMMAND ; i++ )
    {
        if( strstr( command , spotifyNetworkCmd[i].command ) != NULL )
        {
            TRACE_1( NETWORKCOMMAND , "Command found, id: %d" , i );

            if( spotifyNetworkCmd[i].specificArg != NULL )
                memcpy( specificArg , spotifyNetworkCmd[i].specificArg , strlen( spotifyNetworkCmd[i].specificArg ) );

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
    char mainArg[64];
    char *arg = strrchr( command , '#');
    char *arg2 = NULL;
    char specificArg[255];

    memset( query , 0 , MAX_QUERY_LENGTH );
    memset( specificArg , 0 , 255 );
    memset( mainArg , 0 , 64 );

    idFuncptr = searchAction( command , specificArg );

    /* Searching the arguments */
    arg2 = strrchr( command , '%' );

    arg++;

    if( arg2 == NULL )
    {
        TRACE_WARNING( NETWORKCOMMAND , "Arg2 is NULL");

        memcpy( mainArg , arg , strlen( arg ) );
    }
    else
    {
        arg2++;

        /* At this moment the arg one is like blabla%ad because we split using '#' and not '%'. So we have to remove the "%ad" part.*/

        while( *arg != '%' )
        {
            if( *arg == '%' )
                break;

            memcpy( mainArg + strlen( mainArg ) , arg , sizeof( char ) );
            arg++;
        }
    }

    TRACE_3( NETWORKCOMMAND , "Arg : %s , Arg2 : %s" , mainArg , arg2 );

    if( idFuncptr == PC_ERROR )
    {
        TRACE_ERROR( NETWORKCOMMAND , "Command is not supported.");

        return;
    }

    if( specificArg[0] != 0 )
    {
        sprintf( query , "%s%s" , specificArg , arg );
    }
    else
    {
        sprintf( query , "%s" , mainArg );
    }

    TRACE_1( NETWORKCOMMAND , "Execute query : %s" , query );

    if( ( spotifyNetworkCmd[idFuncptr].executeCommandOneArg == NULL ) && ( spotifyNetworkCmd[idFuncptr].needSession == 1 ) )
        spotifyNetworkCmd[idFuncptr].executeCommandTwoArg( g_session , query );
    else if( spotifyNetworkCmd[idFuncptr].executeCommandOneArg == NULL )
        spotifyNetworkCmd[idFuncptr].executeCommandTwoArg( query , arg2 );
    else if( ( spotifyNetworkCmd[idFuncptr].executeCommandTwoArg == NULL ) && ( spotifyNetworkCmd[idFuncptr].needSession == 1 ) )
        spotifyNetworkCmd[idFuncptr].executeCommandOneArg( g_session );
    else if( spotifyNetworkCmd[idFuncptr].executeCommandTwoArg == NULL )
        spotifyNetworkCmd[idFuncptr].executeCommandOneArg( query );
}
