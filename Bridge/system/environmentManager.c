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

#include "system/environmentManager.h"

void setAdhoc( char *essidName , char *key )
{
    TRACE_2( ENVIRONMENTMANAGER , "setAdhoc( %s , %s )" , essidName , key );

    char cmd[1024];

    TRACE_3( ENVIRONMENTMANAGER , "Starting construct the command.");

    sprintf( cmd , "%s %s %s" , SET_AHDOC_SCRIPT , essidName , key );

    system( cmd );

    TRACE_1( ENVIRONMENTMANAGER , "Command executed.");
}
