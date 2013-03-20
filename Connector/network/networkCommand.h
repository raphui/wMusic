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

#ifndef NETWORKCOMMAND_H
#define NETWORKCOMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network/serverManager.h"
#include "network/streamManager.h"
#include "utils/types.h"
#include "utils/trace.h"

#define NETWORK_COUNT_COMMAND   20
#define MAX_QUERY_LENGTH        1024

typedef struct spotifytNetworkCommand
{

    char *command;
    int ( *func )( void *arg1 );

}spotifytNetworkCommand_t;

void doAction( char *command );

#endif // NETWORKCOMMAND_H
