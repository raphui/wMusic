#ifndef TYPES_H
#define TYPES_H

#define PC_SUCCESS  0
#define PC_ERROR    -1

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
