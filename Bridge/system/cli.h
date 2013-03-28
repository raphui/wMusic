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

#ifndef CLI_H
#define CLI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/trace.h"
#include "utils/types.h"
#include "utils/zmemory.h"
#include "utils/threadManager.h"
#include "network/serverManager.h"
#include "spotify/playQueueManager.h"

#define CLI_COUNT_COMMAND       6
#define SET_CLI_COUNT_COMMAND   1

typedef struct cliCommand
{
    char *command;
    char *( *func )( void );

}cliCommand_t;

typedef struct setCliCommand
{
    char *command;
    char *( *func )( unsigned int , unsigned int );

}setCliCommand_t;

void *doCommand( char *cmd );

#endif // CLI_H
