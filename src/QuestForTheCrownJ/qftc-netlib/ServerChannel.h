#pragma once

#include <WinSock2.h>
#include <future>

#include "AuthStructs.h"
#include "ClientStructs.h"
#include "ServerStructs.h"

namespace qfcnet
{
	class ServerChannel
	{
	public:
		std::function<LauncherLoginResponse(const LauncherLoginInfo&)> handleLoginInfo;
		std::function<std::shared_ptr<ServerPlayerInfo>(const RequestPlayerInfo&)> handleRequestPlayer;

	public:
		ServerChannel(int port);
		~ServerChannel();

	private:
		bool stop_listen;
		std::thread listen_thread;
		SOCKET channel_socket;

		void Listen(int port);
	};
}
