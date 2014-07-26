#pragma once

#include <WinSock2.h>
#include <thread>

#include "AuthStructs.h"

namespace qfcnet
{
	class ServerChannel
	{
	public:
		std::function<LauncherLoginResponse(const LauncherLoginInfo&)> handleLoginInfo;

	public:
		ServerChannel();
		~ServerChannel();

		void Listen(int port);

	private:
		bool stop_listen;

		SOCKET server_socket;
	};
}
