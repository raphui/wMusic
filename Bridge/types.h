#ifndef TYPES_H
#define TYPES_H

#include <pthread.h>

#define PC_SUCCESS  0
#define PC_ERROR    -1


pthread_mutex_t mutexSession;

enum accountManagerState
{
    INVALID_LOGIN_INFO,
    NOT_PRENIUM,
    CONNECTION_ERROR,
    CONNECTION_OK
};

enum searchManagerState
{
    SEARCH_ERROR,
    SEARCH_OK
};

#endif // TYPES_H
