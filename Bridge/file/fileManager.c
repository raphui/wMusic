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

            TRACE_3( FILEMANAGER , "######################################");
            TRACE_3( FILEMANAGER , "RIFF: %s" , wavH.RIFF );
            TRACE_3( FILEMANAGER , "ChunkSize: %d" , wavH.ChunkSize );
            TRACE_3( FILEMANAGER , "WAVE: %s" , wavH.WAVE );
            TRACE_3( FILEMANAGER , "fmt: %s" , wavH.fmt );
            TRACE_3( FILEMANAGER , "Subchunk1Size: %d" , wavH.Subchunk1Size );
            TRACE_3( FILEMANAGER , "AudioFormat: %d" , wavH.AudioFormat );
            TRACE_3( FILEMANAGER , "NumOfchan: %d" , wavH.NumOfChan );
            TRACE_3( FILEMANAGER , "SamplesPerSec: %d" , wavH.SamplesPerSec );
            TRACE_3( FILEMANAGER , "bytesPerSec: %d" , wavH.bytesPerSec );
            TRACE_3( FILEMANAGER , "blockAlign: %d" , wavH.blockAlign );
            TRACE_3( FILEMANAGER , "bitsPerSample: %d" , wavH.bitsPerSample );
            TRACE_3( FILEMANAGER , "Subchunk2ID: %s" , wavH.Subchunk2ID );
            TRACE_3( FILEMANAGER , "Subchunk2Size: %d" , wavH.Subchunk2Size );
            TRACE_3( FILEMANAGER , "######################################");

            wavH.ChunkSize = 36 + wavH.Subchunk2Size;
            wavH.Subchunk1Size = 16;
            wavH.AudioFormat = 1;
            wavH.NumOfChan = 2;
            wavH.SamplesPerSec = 44100;
            wavH.bytesPerSec = wavH.SamplesPerSec * wavH.NumOfChan * 16 / 8;
            wavH.blockAlign = wavH.NumOfChan * 16 / 8;
            wavH.bitsPerSample = 16;
            memcpy( wavH.Subchunk2ID , "data" , 4 );


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
