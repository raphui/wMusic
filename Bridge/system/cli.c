#include "cli.h"

static cliCommand_t cliCmd[] =
{
    {"MEMORY_COUNT"     ,   &getMemoryCount },
    {"THREAD_COUNT"     ,   &getThreadCount }
};

static int searchAction( const char *cmd )
{
    TRACE_2( CLI , "searchAction( %s )." , cmd );

    int i = 0;

    for( i = 0 ; i < CLI_COUNT_COMMAND ; i++ )
    {
        if( strstr( cmd , cliCmd[i].command ) != NULL )
        {
            TRACE_1( CLI , "Command found, id: %d" , i );

            return ( int )cliCmd[i].func;
        }
    }

    return PC_ERROR;
}


int doCommand( const char *cmd )
{
    TRACE_2( CLI , "doCommand( %s )." , cmd );

    int ret = 0;

    int ( *func )( void ) = searchAction( cmd );

    if( *func == PC_ERROR )
    {
        TRACE_ERROR( CLI , "Command is not supported.");

        ret = PC_ERROR;
    }
    else
    {
        TRACE_1( CLI , "Execute function.");

        ret = func();

        TRACE_1( CLI , "Function return: %d" , ret );
    }

    return ret;
}
