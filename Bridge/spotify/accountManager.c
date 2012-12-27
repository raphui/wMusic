#include "accountManager.h"

int signin( sp_session *session , const char *username , const char *password )
{
    TRACE_2( ACCOUNTMANAGER , "signin().");

    int next_timeout = 0;

    sp_error error;

    TRACE_3( ACCOUNTMANAGER , "Trying to login....");

    login = FALSE;

    error = sp_session_login( session , username , password , REMEMBER_LOGIN , NULL );

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( ACCOUNTMANAGER , "Fail to login, reason: %s" , sp_error_message( error ) );

        return CONNECTION_ERROR;
    }
    else
    {
        TRACE_2( ACCOUNTMANAGER , "Connection ok !");
    }

    while( login != TRUE )
    {
        sp_session_process_events( session , &next_timeout );
    }

    return CONNECTION_OK;
}

void logged_in( sp_session *session , sp_error error )
{

    TRACE_2( ACCOUNTMANAGER , "logged_in()");

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( ACCOUNTMANAGER , "Fail to login, reason: %s." ,  sp_error_message( error ) );

        exit( 1 );
    }

    TRACE_3( ACCOUNTMANAGER , "Success to login.");

    login = TRUE;
}

void logged_out( sp_session *session )
{
    TRACE_2( SPOTIFYMANAGER , "logged_out()");

}
