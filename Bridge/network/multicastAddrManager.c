#include "multicastAddrManager.h"

static char addrUsed[MAX_ADDR_IN_USED][14];

static int countAddrUsed = 0;
static int firstDigit = 224;
static int secondDigit = 0;
static int thirdDigit = 0;
static int fourthDigit = 0;

int getNextMulticastAddr( char *addr )
{
    TRACE_2( MULTICASTADDRMANAGER , "getNextMulticastAddr().");

    int ret;

    secondDigit++;
    thirdDigit++;
    fourthDigit++;

    ret = sprintf( addr , "%d.%d.%d.%d" , firstDigit , secondDigit , thirdDigit , fourthDigit );

    if( ret < 0 )
    {
        TRACE_ERROR( MULTICASTADDRMANAGER , "Cannot generate the new multicast addr , sprintf failed.");

        return PC_ERROR;
    }

    TRACE_3( MULTICASTADDRMANAGER , "Verify if the addr is already used.");

    ret = checkAddr( addr );

    if( ret == ADDR_ALREADY_USED )
    {
        TRACE_ERROR( MULTICASTADDRMANAGER , "The address is already used !");
    }
    else if( ret == ADDR_OK )
    {
        TRACE_3( MULTICASTADDRMANAGER , "The address can be used !");
    }

    TRACE_3( MULTICASTADDRMANAGER , "Saving the addr in the address currently used.");

    saveAddr( addr );

    countAddrUsed++;

    return PC_SUCCESS;
}

void saveAddr( const char *addr )
{
    TRACE_2( MULTICASTADDRMANAGER , "saveAddr( %s )." , addr );

    strcpy( addrUsed[countAddrUsed] , addr );
}

int checkAddr( const char *addr )
{
    TRACE_2( MULTICASTADDRMANAGER , "checkAddr( %s )." , addr );

    int i = 0;
    int ret = ADDR_OK;

    for( i = 0 ; i < MAX_ADDR_IN_USED ; i++ )
        if( strcmp( addrUsed[i] , addr ) == 0 )
            ret = ADDR_ALREADY_USED;

    return ret;
}
