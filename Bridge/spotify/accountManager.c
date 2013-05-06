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

#include "accountManager.h"

static sp_session *currentSession;

void initAccountManager( sp_session *session )
{
    TRACE_2( ACCOUNTMANAGER , "initAccountManager().");

    currentSession = session;
}

int login( const char *username , const char *password )
{
    TRACE_2( ACCOUNTMANAGER , "login( %s , %s )." , username , password );

    int next_timeout = 0;

    sp_error error;

    TRACE_3( ACCOUNTMANAGER , "Trying to login...");

    logged = FALSE;

    LOCK_MUTEX( ACCOUNTMANAGER , &mutexSession );

    error = sp_session_login( currentSession , username , password , REMEMBER_LOGIN , NULL );

    UNLOCK_MUTEX( ACCOUNTMANAGER , &mutexSession );

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( ACCOUNTMANAGER , "Fail to login, reason: %s" , sp_error_message( error ) );

        return CONNECTION_ERROR;
    }
    else
    {
        TRACE_1( ACCOUNTMANAGER , "Connection on the way...");
    }

    return CONNECTION_OK;

}

int logout( sp_session *session )
{
    TRACE_2( ACCOUNTMANAGER , "logout().");

    int status = PC_SUCCESS;

    sp_error error;

    LOCK_MUTEX( ACCOUNTMANAGER , &mutexSession );

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

    UNLOCK_MUTEX( ACCOUNTMANAGER , &mutexSession );

    return status;
}

void logged_in( sp_session *session , sp_error error )
{

    TRACE_2( ACCOUNTMANAGER , "logged_in()");

    char response[255] = { 0 };

    if( error != SP_ERROR_OK )
    {
        TRACE_ERROR( ACCOUNTMANAGER , "Fail to login, reason: %s." ,  sp_error_message( error ) );

        snprintf( response , 255 , "NOK : %s" , sp_error_message( error ) );

        sendVoid( ( void * )response , strlen( response ) );
    }
    else
    {
        TRACE_1( ACCOUNTMANAGER , "Success to login.");

        if( initPlaylistManager( currentSession ) == PC_ERROR )
        {
            TRACE_ERROR( ACCOUNTMANAGER , "Fail init the playlist manager !");
        }

        logged = TRUE;

        TRACE_INFO( ACCOUNTMANAGER , "Hello !");

        snprintf( response , 255 , "OK");

        sendVoid( ( void * )response , strlen( response ) );

    }

}

void logged_out( sp_session *session )
{
    TRACE_2( SPOTIFYMANAGER , "logged_out()");

    char response[25] = { 0 };

    sprintf( response , "OK");

    sendVoid( ( void * )response , strlen( response ) );

    TRACE_INFO( ACCOUNTMANAGER , "Goodbye !");
}
