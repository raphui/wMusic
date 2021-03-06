/*  wMusic - Music system software
 *  Copyright (C) 2013  Raphaël POGGI
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "trace.h"


static char *traceLevelToChar( unsigned int level );

typedef struct trace_module
{
    char module[35];
    int level;

}trace_module;


trace_module trace_modules[] =
{
    {"THREADMANAGER"        ,   TRACE_LEVEL_ALL },
    {"SPOTIFYMANAGER"       ,   TRACE_LEVEL_ALL },
    {"NETWORKCOMMAND"       ,   TRACE_LEVEL_ALL },
    {"PLAYERMANAGER"        ,   TRACE_LEVEL_ALL },
    {"ACCOUNTMANAGER"       ,   TRACE_LEVEL_ALL },
    {"PLAYLISTMANAGER"      ,   TRACE_LEVEL_ALL },
    {"SEARCHMANAGER"        ,   TRACE_LEVEL_ALL },
    {"SERVERMANAGER"        ,   TRACE_LEVEL_ALL },
    {"VLCMANAGER"           ,   TRACE_LEVEL_ALL },
    {"FILEMANAGER"          ,   TRACE_LEVEL_ALL },
    {"ENVIRONMENTMANAGER"   ,   TRACE_LEVEL_ALL },
    {"PLAYQUEUE"            ,   TRACE_LEVEL_ALL },
    {"PLAYQUEUEMANAGER"     ,   TRACE_LEVEL_ALL },
    {"ZMEMORY"              ,   TRACE_LEVEL_ALL },
    {"CLI"                  ,   TRACE_LEVEL_ALL },
    {"MULTICASTADDRMANAGER" ,   TRACE_LEVEL_ALL },
    {"STREAMMANAGER"        ,   TRACE_LEVEL_ALL }
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

    if( module >= MODULE_COUNT )
    {
        sprintf( buff , "This module does not exist.\n");
    }
    else if( trace_modules[module].level == level )
    {
        /* Do nothing */
        sprintf( buff , "The module have already this level of trace.\n");
    }
    else if( ( level != TRACE_LEVEL_ALL )
             && ( level != TRACE_LEVEL_MID )
             && ( level != TRACE_LEVEL_DEFAULT )
             && ( level != TRACE_LEVEL_FUNC ) )
    {
        /* Do nothing */
        sprintf( buff , "This level of trace doesn't exist.\n");
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
        case TRACE_LEVEL_MID:
            return "MID";
        case TRACE_LEVEL_FUNC:
            return "FUNC";
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

char *levelInfo( void )
{
    char *buff = ( char * )zmalloc( 1024 * sizeof( char ) );

    int i = 0;

    /* memset to clean the buff, because not all the buff will be fill.*/
    memset( buff , 0 , 1024 );

    sprintf( buff , "%d:%s\n%d:%s\n%d:%s\n%d:%s\n"  , TRACE_LEVEL_ALL , traceLevelToChar( TRACE_LEVEL_ALL )
                                                    , TRACE_LEVEL_MID , traceLevelToChar( TRACE_LEVEL_MID )
                                                    , TRACE_LEVEL_FUNC , traceLevelToChar( TRACE_LEVEL_FUNC )
                                                    , TRACE_LEVEL_DEFAULT , traceLevelToChar( TRACE_LEVEL_DEFAULT ) );

    return buff;
}
