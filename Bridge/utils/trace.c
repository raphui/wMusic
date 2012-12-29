#include "trace.h"


static char *traceLevelToChar( unsigned int level );

typedef struct trace_module
{
    char module[35];
    int level;

}trace_module;


trace_module trace_modules[] =
{
    {"STREAMINGSERVER"      ,   TRACE_LEVEL_DEFAULT },
    {"COMMANDERSERVER"      ,   TRACE_LEVEL_DEFAULT },
    {"THREADMANAGER"        ,   TRACE_LEVEL_DEFAULT },
    {"SPOTIFYMANAGER"       ,   TRACE_LEVEL_DEFAULT },
    {"NETWORKCOMMAND"       ,   TRACE_LEVEL_DEFAULT },
    {"PLAYERMANAGER"        ,   TRACE_LEVEL_DEFAULT },
    {"ACCOUNTMANAGER"       ,   TRACE_LEVEL_DEFAULT },
    {"PLAYLISTMANAGER"      ,   TRACE_LEVEL_DEFAULT },
    {"SEARCHMANAGER"        ,   TRACE_LEVEL_DEFAULT },
    {"SERVERMANAGER"        ,   TRACE_LEVEL_DEFAULT },
    {"VLCMANAGER"           ,   TRACE_LEVEL_DEFAULT },
    {"FILEMANAGER"          ,   TRACE_LEVEL_DEFAULT },
    {"ENVIRONMENTMANAGER"   ,   TRACE_LEVEL_DEFAULT },
    {"PLAYQUEUE"            ,   TRACE_LEVEL_DEFAULT },
    {"PLAYQUEUEMANAGER"     ,   TRACE_LEVEL_DEFAULT },
    {"ZMEMORY"              ,   TRACE_LEVEL_ALL },
    {"CLI"                  ,   TRACE_LEVEL_DEFAULT },
    {"MULTICASTADDRMANAGER" ,   TRACE_LEVEL_DEFAULT }

};

void Traces_printOnly(unsigned int level, unsigned int module, const char *function, const char *format, ...)
{

}

void Traces_print( const char *file , const int line , const char *function , unsigned int level , unsigned int module ,  const char *format , ... )
{

    va_list args;
    time_t rawtime;
    struct tm *timeinfo;

    int i = 1;
    char *timestr = NULL;

    time( &rawtime );

    timeinfo = localtime( &rawtime );

    if( ( trace_modules[module].level & level ) == level )
    {
        va_start( args , format );

        char buff[255];

        memset( buff , 0 , 255 );

        const char *tmp = strrchr( file , '/');

        timestr = asctime( timeinfo );

        strtok( timestr , " ");

        while( i++ != 3 )
            strtok( NULL , " ");

        timestr = strtok( NULL , " ");

        printf("%s" , timestr );

        if( level == TRACE_LEVEL_WARN )
        {
            printf("[WARN]");
        }
        else if( level == TRACE_LEVEL_ERR )
        {
            printf("[ERR]");
        }
        else if( level == TRACE_LEVEL_INFO )
        {
            printf("[INFO]");
        }

        printf("[%s][%s]{%s:%d} : " , trace_modules[module].module , function , ( tmp ) ? ++tmp : file , line );

        vsprintf( buff , format , args );

        printf("%s" , buff );

        va_end( args );

        printf("\n");

    }

}

int Traces_enable(unsigned int module, unsigned int level )
{
    if( ( trace_modules[module].level & level ) == level )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


char *setTraceLevel( unsigned int module , unsigned int level )
{
    char *buff = ( char * )zmalloc( 248 * sizeof( char ) );

    memset( buff , 0 , 248 );

    if( trace_modules[module].level == level )
    {
        /* Do nothing */
        sprintf( buff , "The module have already this level of trace.\n");
    }
    else
    {
        trace_modules[module].level = level;

        sprintf( buff , "The trace level of the module changed.\n");
    }

    return buff;
}

static char *traceLevelToChar( unsigned int level )
{
    switch( level )
    {
        case TRACE_LEVEL_DEFAULT:
            return "DEFAULT";
        case TRACE_LEVEL_ALL:
            return "ALL";
    }

    return "UNKNOWN";
}

char *dumpTrace( void )
{
    char *buff = ( char * )zmalloc( 1024 * sizeof( char ) );

    int i = 0;

    /* memset to clean the buff, because not all the buff will be fill.*/
    memset( buff , 0 , 1024 );

    for( i = 0 ; i < MODULE_COUNT ; i++ )
    {
        sprintf( buff + strlen( buff ) , "%d:%s\t\t\t%d:%s\n" , i , trace_modules[i].module , trace_modules[i].level , traceLevelToChar( trace_modules[i].level ) );
    }

    return buff;
}
