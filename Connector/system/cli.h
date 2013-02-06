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

#define CLI_COUNT_COMMAND       5
#define SET_CLI_COUNT_COMMAND   1

typedef struct cliCommand
{
    char *command;
    char *( *func )( void );

}cliCommand_t;

typedef struct setCliCommand
{
    char *command;
    char *( *func )( void* , void* );

}setCliCommand_t;

void *doCommand( char *cmd );

#endif // CLI_H
