#ifndef ZMALLOC_H
#define ZMALLOC_H

#include <stdio.h>
#include <stdlib.h>

#include "utils/trace.h"

void *zmalloc( size_t size );
int getMemoryCount( void );

#endif // ZMALLOC_H
