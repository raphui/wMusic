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

#include "streamManager.h"

void setDispatcherIp( char *ip )
{
    TRACE_2( STREAMMANAGER , "setDispatcherIp( %s )." , ip );

    size_t size = strlen( ip );

    dispatcherIp = ( const char * )zmalloc( size * sizeof( char ) );

    //snprintf( dispatcherIp , size , "%s" , ip );

    memcpy( dispatcherIp , ip , size );
}

int playStreamer( char *url )
{
    TRACE_2( STREAMMANAGER , "playStreamer( %s )." , url );

    int status = PC_SUCCESS;
    int ret;

    ret = playUrl( url );

    if( ret == PC_ERROR )
    {
        TRACE_ERROR( STREAMMANAGER , "Fail to play the url : %s" , url );

        status = PC_ERROR;
    }

    return status;
}
