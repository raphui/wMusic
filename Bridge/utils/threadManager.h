#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <unistd.h>

#include "utils/types.h"
#include "utils/trace.h"

void createThread( void *routine , void *arg );
int getThreadCount( void );
void releaseThread( void );

#endif // THREADMANAGER_H
