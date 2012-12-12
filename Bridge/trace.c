#include "trace.h"


typedef struct trace_module
{
    char module[35];
    int level;

}trace_module;


trace_module trace_modules[] =
{
    {"PLAYER"           ,   TRACE_LEVEL_DEFAULT },
    {"STREAMINGSERVER"  ,   TRACE_LEVEL_DEFAULT },
    {"COMMANDERSERVER"  ,   TRACE_LEVEL_DEFAULT },
    {"THREADMANAGER"    ,   TRACE_LEVEL_DEFAULT },
    {"SPOTIFYMANAGER"   ,   TRACE_LEVEL_DEFAULT },
    {"NETWORKCOMMAND"   ,   TRACE_LEVEL_DEFAULT },
    {"PLAYERMANAGER"    ,   TRACE_LEVEL_DEFAULT },
    {"ACCOUNTMANAGER"   ,   TRACE_LEVEL_DEFAULT },
    {"PLAYLISTMANAGER"  ,   TRACE_LEVEL_DEFAULT },
    {"SEARCHMANAGER"    ,   TRACE_LEVEL_DEFAULT },
    {"SERVERMANAGER"    ,   TRACE_LEVEL_DEFAULT }

};

void Traces_printOnly(unsigned int level, unsigned int module, const char *function, const char *format, ...)
{

}

void Traces_print( const char *file , const int line , const char *function , unsigned int level , unsigned int module ,  const char *format , ... )
{

    if( ( trace_modules[module].level & level ) == level )
    {
        va_list args;
        va_start( args , format );

        char buff[255];

        memset( buff , 0 , 255 );

        const char *tmp = strrchr( file , '/');

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

