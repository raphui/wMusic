#include "system/environmentManager.h"

void setAdhoc( char *essidName , char *key )
{
    TRACE_2( ENVIRONMENTMANAGER , "setAdhoc( %s , %s )" , essidName , key );

    char cmd[1024];

    TRACE_3( ENVIRONMENTMANAGER , "Starting construct the command.");

    strcat( cmd , SET_AHDOC_SCRIPT );
    strcat( cmd , " ");
    strcat( cmd , essidName );
    strcat( cmd , " ");
    strcat( cmd , key );

    system( cmd );
}
