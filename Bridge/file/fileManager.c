#include "fileManager.h"

int createFile( void )
{
    TRACE_2( FILEMANAGER , "createFile()");

    FILE *f = NULL;
    FILE *h = NULL;
    wavHeader_t wavH;

    size_t ret;

    int status = PC_SUCCESS;

    f = fopen( STREAM_FILE , "wb+");
    h = fopen( REF_FILE , "rb");

    if( f == NULL || h == NULL )
    {
        TRACE_ERROR( FILEMANAGER , "f or h FILE is NULL");

        status = PC_ERROR;
    }
    else
    {
        ret = fread( &wavH , sizeof( wavH ) , 1 ,  h );

        if( ret != 1 )
        {
            TRACE_ERROR( FILEMANAGER , "Fail to read all data.");

            status = PC_ERROR;
        }
        else
        {
            ret = fwrite( &wavH , sizeof( wavH ) , 1 , f );

            if( ret != 1 )
            {
                TRACE_ERROR( FILEMANAGER , "Fail to write all data.");

                status = PC_ERROR;
            }
        }

        fclose( f );
        fclose( h );
    }

    return status;
}

int writeFile( void *data )
{
    TRACE_2( FILEMANAGER , "writeFile()");

    static int firstTime = 0;
    int status = PC_SUCCESS;

    size_t ret;

    FILE *f = NULL;

    f = fopen( STREAM_FILE , "ab");

    if( f == NULL )
    {
        TRACE_ERROR( FILEMANAGER , "f FILE is NULL");

        status = PC_ERROR;
    }
    else
    {
        ret = fwrite( data , DATA_SIZE , 1 , f );

        if( ret != 1 )
        {
            TRACE_ERROR( FILEMANAGER , "Not all data have been written.");

            status = PC_ERROR;
        }
        else
        {
            TRACE_3( FILEMANAGER , "Data have been written.");

            fclose( f );

            if( firstTime == 0 )
            {
                TRACE_3( FILEMANAGER , "Start to stream file.");

                streamFile( STREAM_FILE );

                firstTime++;
            }
        }
    }

    return status;
}
