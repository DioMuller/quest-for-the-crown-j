#pragma once

#include <functional>
#include <WinSock2.h>
#include <string>
#include <vector>
#include <memory>
#include <future>
#include <thread>

#include "AuthStructs.h"
#include "StructBase.h"
#include "ServerStructs.h"

namespace qfcnet
{
	class ClientChannel
	{
	public:
		std::string auth_token;
		std::function<void(const ServerEntityInfo&)> onEntity;

	public:
		ClientChannel(std::string serverAddress, int port);

		~ClientChannel();

		std::future<std::string> Login(std::string user, std::string password);

		ServerPlayerInfo GetPlayerInfo();
		void GetEntities(std::string screen_name);

	private:
		std::thread listen_thread;
		bool stop_listen;
		SOCKET channel_socket;

		sockaddr_in server_addr;
		int server_addr_size;

		std::function<void(s_launcher_login_response)> on_login_response;

		void Listen();
	};
}
