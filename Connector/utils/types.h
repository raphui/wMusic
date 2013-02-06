#ifndef TYPES_H
#define TYPES_H

#include <pthread.h>

#define PC_SUCCESS          0
#define PC_ERROR            -1

#define TRUE                1
#define FALSE               0

/**** Compilation Environment settings ****/
#define CROSS_COMPILE       0

/**** System Environment settings ****/
#define SET_ENV             0
#define ESSID               "wMusic"
#define KEY                 "azert"

enum
{
    SUCCESS =   0x0,
    ERROR   =   0x80
};


#endif // TYPES_H
