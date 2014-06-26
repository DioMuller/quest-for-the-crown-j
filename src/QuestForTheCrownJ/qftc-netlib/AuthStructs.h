#pragma once
#include <string>
#include "StructBase.h"

// Login info (sent from launcher).
typedef struct s_launcher_login_info
{
	Header header;
	std::string login;
	std::string hashedPassword;
} LauncherLoginInfo;

// Login info (sent from server).
typedef struct s_launcher_login_response
{
	Header header;
	bool authenticated;
	std::string authKey;
} LauncherLoginResponse;