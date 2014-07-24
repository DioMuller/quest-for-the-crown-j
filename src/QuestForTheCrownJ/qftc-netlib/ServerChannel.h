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
		ServerChannel(int port);
		~ServerChannel();
	private:
		void Listen();

		std::thread listen_thread;
		bool stop_listen;

		SOCKET server_socket;
	};
}
