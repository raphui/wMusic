#ifndef MULTICASTADDRMANAGER_H
#define MULTICASTADDRMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/trace.h"
#include "utils/types.h"

#define MAX_ADDR_IN_USED    10
#define ADDR_ALREADY_USED   ( 1 << 2 )
#define ADDR_OK             ( 1 << 3 )

int getNextMulticastAddr( char *addr );
void saveAddr( const char *addr );
int checkAddr( const char *addr );

#endif // MULTICASTADDRMANAGER_H
