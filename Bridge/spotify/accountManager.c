#include "accountManager.h"

static sp_session *currentSession;

void initAccountManager( sp_session *session )
{
    TRACE_2( ACCOUNTMANAGER , "initAccountManager().");

    currentSession = session;
}

int signin( sp_session *session , const char *username , const char *password )
{
    TRACE_2( ACCOUNTMANAGER , "signin().");

    int next_timeout = 0;

    sp_error error;

    TRACE_3( ACCOUNTMANAGER , "Trying to login....");

    logged = FALSE;

    error = sp_session_login( session , username , password , REMEMBER_LOGIN , NULL );

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( ACCOUNTMANAGER , "Fail to login, reason: %s" , sp_error_message( error ) );

        return CONNECTION_ERROR;
    }
    else
    {
        TRACE_1( ACCOUNTMANAGER , "Connection ok !");
    }

    while( logged != TRUE )
    {
        sp_session_process_events( session , &next_timeout );
    }

    return CONNECTION_OK;
}

int login( const char *username , const char *password )
{
    TRACE_2( ACCOUNTMANAGER , "login( %s , %s )." , username , password );

    int next_timeout = 0;

    sp_error error;

    TRACE_3( ACCOUNTMANAGER , "Trying to login...");

    logged = FALSE;

//    pthread_mutex_lock( &mutexSession );

    error = sp_session_login( currentSession , username , password , REMEMBER_LOGIN , NULL );

//    pthread_mutex_unlock( &mutexSession );

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( ACCOUNTMANAGER , "Fail to login, reason: %s" , sp_error_message( error ) );

        return CONNECTION_ERROR;
    }
    else
    {
        TRACE_1( ACCOUNTMANAGER , "Connection ok !");
    }

    while( logged != TRUE )
    {
//        pthread_mutex_lock( &mutexSession );

        sp_session_process_events( currentSession , &next_timeout );

//        pthread_mutex_unlock( &mutexSession );
    }

    return CONNECTION_OK;

}

int logout( sp_session *session )
{
    TRACE_2( ACCOUNTMANAGER , "logout().");

    int status = PC_SUCCESS;

    sp_error error;

    pthread_mutex_lock( &mutexSession );

    error = sp_session_logout( session );

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( ACCOUNTMANAGER , "Fail to logout, reason : %s" , sp_error_message( error ) );

        status = PC_ERROR;
    }
    else
    {
        TRACE_1( ACCOUNTMANAGER , "Success to logout.");
    }

    pthread_mutex_unlock( &mutexSession );

    return status;
}

void logged_in( sp_session *session , sp_error error )
{

    TRACE_2( ACCOUNTMANAGER , "logged_in()");

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( ACCOUNTMANAGER , "Fail to login, reason: %s." ,  sp_error_message( error ) );

        exit( 1 );
    }

    TRACE_1( ACCOUNTMANAGER , "Success to login.");

    if( initPlaylistManager( currentSession ) == PC_ERROR )
    {
        TRACE_ERROR( ACCOUNTMANAGER , "Fail to init the playlist manager !");
    }

    logged = TRUE;

    TRACE_INFO( ACCOUNTMANAGER , "Hello !");
}

void logged_out( sp_session *session )
{
    TRACE_2( SPOTIFYMANAGER , "logged_out()");

    TRACE_INFO( ACCOUNTMANAGER , "Goodbye !");
}
