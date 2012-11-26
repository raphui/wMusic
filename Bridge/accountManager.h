#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libspotify/api.h>

#include "types.h"
#include "trace.h"

/* ############# CALLBACK functions ############# */
void logged_in( sp_session *session , sp_error error);
void logged_out( sp_session *session );


int signin( sp_session *session , const char *username , const char *password );

static int login;

#endif // ACCOUNTMANAGER_H