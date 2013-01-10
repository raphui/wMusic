#ifndef NETWORKCOMMAND_H
#define NETWORKCOMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spotify/playerManager.h"
#include "spotify/searchManager.h"
#include "network/serverManager.h"
#include "utils/types.h"
#include "utils/trace.h"

#define NETWORK_COUNT_COMMAND   14
#define MAX_QUERY_LENGTH        1024

typedef struct spotifytNetworkCommand
{

    char *command;
    int ( *executeCommandOneArg )( void *arg1 );
    int ( *executeCommandTwoArg )( void *arg1 , void *arg2 );
    char *specificArg;
    int needSession;

}spotifytNetworkCommand_t;

void doAction( char *command );

#endif // NETWORKCOMMAND_H
