#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <unistd.h>

#include "types.h"
#include "trace.h"

void createThread( void *routine , void *arg );

#endif // THREADMANAGER_H
