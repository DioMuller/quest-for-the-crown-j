#pragma once
#include <string>
#include "StructBase.h"

// Login info (sent from launcher).
typedef struct s_launcher_login_info
{
	Header header;
	char login[50];
	char hashedPassword[260];
} LauncherLoginInfo;

// Login info (sent from server).
typedef struct s_launcher_login_response
{
	Header header;
	bool authenticated;
	char authKey[260];
} LauncherLoginResponse;