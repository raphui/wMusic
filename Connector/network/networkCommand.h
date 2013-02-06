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
