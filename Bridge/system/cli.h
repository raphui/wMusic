#ifndef CLI_H
#define CLI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/trace.h"
#include "utils/types.h"
#include "utils/zmalloc.h"
#include "utils/threadManager.h"
#include "network/serverManager.h"

#define COUNT_COMMAND   2

typedef struct cliCommand
{
    char *command;
    int ( *func )( void );

}cliCommand_t;

int doCommand( const char *cmd );

#endif // CLI_H
