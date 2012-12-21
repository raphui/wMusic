#ifndef ENVIRONMENTMANAGER_H
#define ENVIRONMENTMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/types.h"
#include "utils/trace.h"

#define SET_AHDOC_SCRIPT    "Scripts/set_ad-hoc.sh"


void setAdhoc( char *essidName , char *key );

#endif // ENVIRONMENTMANAGER_H
