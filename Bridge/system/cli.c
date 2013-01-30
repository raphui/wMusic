#include "cli.h"

static char *help( void );

static cliCommand_t cliCmd[] =
{
    {"memory_count"     ,   &getMemoryCount },
    {"thread_count"     ,   &getThreadCount },
    {"dump_trace_level" ,   &dumpTrace      },
    {"get_level"        ,   &levelInfo      },
    {"dump_playqueue"   ,   &dumpPlayQueue  },
    {"help"             ,   &help           }
};

static setCliCommand_t setCliCmd[] =
{
    {"set_trace_level"  ,   &setTraceLevel  }
};

static char *help( void )
{
    TRACE_2( CLI , "help().");

    int i = 0;
    char *buff = ( char * )zmalloc( 1024 * sizeof( char ) );

    memset( buff , 0 , 1024 );

    for( i = 0 ; i < CLI_COUNT_COMMAND ; i++ )
        sprintf( buff + strlen( buff ) , "%s\n" ,  cliCmd[i].command );

    for( i = 0 ; i < SET_CLI_COUNT_COMMAND ; i++ )
        sprintf( buff + strlen( buff ) , "%s\n" ,  setCliCmd[i].command );

    return buff;
}

static int searchCliCmd( const char *cmd )
{
    TRACE_2( CLI , "searchCliCmd( %s )." , cmd );

    int i = 0;

    for( i = 0 ; i < CLI_COUNT_COMMAND ; i++ )
    {
        if( strstr( cmd , cliCmd[i].command ) != NULL )
        {
            TRACE_1( CLI , "Command found, id: %d" , i );

            return i;
        }
    }

    return PC_ERROR;
}

static int searchSetCliCmd( const char *cmd )
{
    TRACE_2( CLI , "searchSetCliCmd( %s )." , cmd );

    int i = 0;

    for( i = 0 ; i < SET_CLI_COUNT_COMMAND ; i++ )
    {
        if( strstr( cmd , setCliCmd[i].command ) != NULL )
        {
            TRACE_1( CLI , "Command found, id: %d" , i );

            return i;
        }
    }

    return PC_ERROR;
}


void *doCommand( char *cmd )
{
    TRACE_2( CLI , "doCommand( %s )." , cmd );

    int idFuncptr;
    unsigned int arg1;
    unsigned int arg2;
    void *ret = NULL;

    /* Dynamic allocation, because after send the response throught the socket, free() is called. (If it's declare like -char errorMsg[]="zedze"- this will crash for sure. */
    char *errorMsg = ( char * )zmalloc( 28 * sizeof( char ) );

    memset( errorMsg , 0 , 28 );

    if( ( strstr( cmd , "set_") == NULL )
            && ( ( idFuncptr = searchCliCmd( cmd ) ) != PC_ERROR ) )
    {
        TRACE_3( CLI , "Execute function.");

        ret = ( void * )cliCmd[idFuncptr].func();

        TRACE_1( CLI , "Function return: %d" , ret );
    }
    else if( ( idFuncptr = searchSetCliCmd( cmd ) ) != PC_ERROR )
    {
        TRACE_3( CLI , "Execute function.");

        strtok( cmd , " ");

        arg1 = atoi( strtok( NULL , " ") );
        arg2 = atoi( strtok( NULL , " ") );

        ret = ( void * )setCliCmd[idFuncptr].func( arg1 , arg2 );

        TRACE_1( CLI , "Function return: %d" , ret );
    }
    else
    {
        TRACE_ERROR( CLI , "Command is not supported.");

        sprintf( errorMsg , "Command is not supported.\n");

        ret = ( void * )errorMsg;
    }

    return ret;
}
