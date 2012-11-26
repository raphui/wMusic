#include "accountManager.h"

int signin( sp_session *session , const char *username , const char *password )
{
    int next_timeout = 0;

    sp_error error;

    printf("Trying to login....\n");

    login = 0;

    error = sp_session_login( session , username , password , 0 , NULL );

    if( error != SP_ERROR_OK )
    {
        printf("Fail to login, reason: %s" , sp_error_message( error ) );

        return -1;
    }

    while( login != 1 )
    {
        usleep( next_timeout * 1000 );

        sp_session_process_events( session , &next_timeout );
    }

    return CONNECTION_OK;
}

void logged_in( sp_session *session , sp_error error )
{

    TRACE_2( SPOTIFYMANAGER , "logged_in()");

    if( error != SP_ERROR_OK )
    {
        printf("Fail to login, reason: %s.\n" ,  sp_error_message( error ) );

        exit( 1 );
    }

    printf("Success to login.\n");

    login = 1;
}

void logged_out( sp_session *session )
{
    TRACE_2( SPOTIFYMANAGER , "logged_out()");

}
