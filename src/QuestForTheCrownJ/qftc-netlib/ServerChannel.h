#pragma once

#include <WinSock2.h>
#include <future>

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

		std::future<void> ListenAsync(int port);
		void Listen(int port);

	private:
		bool stop_listen;
		SOCKET channel_socket;
	};
}
