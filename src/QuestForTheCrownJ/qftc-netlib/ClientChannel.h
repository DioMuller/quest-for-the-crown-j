#pragma once
#include <functional>
#include <WinSock2.h>
#include <string>
#include <vector>
#include <memory>
#include "StructBase.h"
#ifndef _M_CEE
#include <thread>
#endif
#include "ServerStructs.h"

namespace qfcnet
{
	class ClientChannel
	{
	public:
		std::string auth_token;
		std::function<void(ServerEntityInfo)> onEntity;

	public:
		ClientChannel(std::string serverAddress, int port);
		~ClientChannel();

		void Login(std::string user, std::string password);

		ServerPlayerInfo GetPlayerInfo();
		void GetEntities(std::string screen_name);

	private:
#ifndef _M_CEE
		void Listen();

		std::thread listen_thread;
		bool stop_listen;

		SOCKET server_socket;
		sockaddr_in server_addr;
		int server_addr_size;
#endif
	};
}
