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
		std::function<LauncherLoginResponse(const LauncherLoginInfo&, sockaddr_in, int)> handleLoginInfo;
		std::function<std::shared_ptr<ServerResponsePlayerInfo>(const ClientRequestPlayerInfo&)> handleRequestPlayer;
		std::function<void(const ClientSendPlayerPosition&)> handlePlayerPosition;
		std::function<void(const ClientSendPlayerFullPosition&)> handlePlayerFullPosition;
		std::function<void(const ClientRequestEntities&)> handleRequestEntities;

	public:
		ServerChannel(int port);
		~ServerChannel();

		template <typename T>
		void Send(T data, sockaddr_in addr, int addr_size)
		{
			int r = sendto(channel_socket, (char*)&data, sizeof(data), 0, (SOCKADDR*)&addr, addr_size);
			if (r <= 0)
				Log::Error("Send error: " + std::to_string(WSAGetLastError()));
		}

		void SendEntity(int id, EntityType type, int x, int y, sockaddr_in addr, int addr_size);
	private:
		bool stop_listen;
		std::thread listen_thread;
		SOCKET channel_socket;

		void Listen(int port);
	};
}
