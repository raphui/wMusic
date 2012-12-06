#ifndef NETWORKCOMMAND_H
#define NETWORKCOMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "playerManager.h"
#include "searchManager.h"
#include "serverManager.h"
#include "types.h"
#include "trace.h"

#define COUNT_COMMAND   6

typedef struct networkCommand
{

    const char *command;
    int ( *executeCommand )( sp_session *arg1 , char *arg2 );

}networkCommand_t;


void doAction( char *command );

#endif // NETWORKCOMMAND_H
