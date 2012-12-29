#include "cli.h"

static cliCommand_t cliCmd[] =
{
    {"memory_count"     ,   &getMemoryCount },
    {"thread_count"     ,   &getThreadCount },
    {"dump_trace_level" ,   &dumpTrace      }
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

//            return ( int )cliCmd[i].func;

            return i;
        }
    }

    return PC_ERROR;
}


void *doCommand( const char *cmd )
{
    TRACE_2( CLI , "doCommand( %s )." , cmd );

    int idFuncptr;
    void *ret = NULL;

    /* Dynamic allocation, because after send the response throught the socket, free() is called. (If it's declare like -char errorMsg[]="zedze"- this will crash for sure. */
    char *errorMsg = ( char * )zmalloc( 28 * sizeof( char ) );

    memset( errorMsg , 0 , 28 );

    idFuncptr = searchAction( cmd );

    if( idFuncptr == PC_ERROR )
    {
        TRACE_ERROR( CLI , "Command is not supported.");

        sprintf( errorMsg , "Command is not supported.\n");

        ret = ( void * )errorMsg;

    }
    else
    {
        TRACE_1( CLI , "Execute function.");

        ret = ( void * )cliCmd[idFuncptr].func();

        TRACE_1( CLI , "Function return: %d" , ret );
    }

    return ret;
}
