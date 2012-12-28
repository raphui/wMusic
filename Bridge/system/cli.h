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

#define CLI_COUNT_COMMAND   3

typedef struct cliCommand
{
    char *command;
    void *( *func )( void );

}cliCommand_t;

void *doCommand( const char *cmd );

#endif // CLI_H
