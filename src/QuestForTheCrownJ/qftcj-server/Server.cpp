#include "Server.h"

#include <string>
#include <sstream>
#include <thread>

#include "Log.h"
#include "StructBase.h"
#include "AuthStructs.h"
#include "ClientStructs.h"
#include "Definitions.h"

using namespace qfcserver;
using namespace qfcbase;

Server::Server(int port)
	: channel(port)
{
	channel.handleLoginInfo = [this](const LauncherLoginInfo& info){
		return HandleLoginInfo(info);
	};
}


Server::~Server()
{
}


void Server::Run()
{
	UpdateLoop();
}

void Server::UpdateLoop()
{
	//Initial update duration = 1 frame.
	double delta = SECONDS_PER_FRAME;

	while (true)
	{
		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();

		// foreach player screen
		//screen->Update(delta);

		end = std::chrono::system_clock::now();

		delta = ((double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / (1000.0 * 1000.0);

		if (delta < SECONDS_PER_FRAME)
		{
			std::this_thread::sleep_for(std::chrono::microseconds((long long)((SECONDS_PER_FRAME - delta) * (1000 * 1000))));
			delta = SECONDS_PER_FRAME;
		}
	}
}

LauncherLoginResponse Server::HandleLoginInfo(const LauncherLoginInfo& login_info)
{
	std::stringstream logBuilder;
	logBuilder << "Login: " << login_info.login << " (" << login_info.hashedPassword << ")";
	Log::Message(logBuilder.str());
	s_launcher_login_response resp;
	ZeroMemory(&resp, sizeof(resp));
	resp.authenticated = true;

	if (resp.authenticated)
		Log::Debug("User authenticated");
	else
		Log::Debug("Invalid username / password");

	return resp;
}
