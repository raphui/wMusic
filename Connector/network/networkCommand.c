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
