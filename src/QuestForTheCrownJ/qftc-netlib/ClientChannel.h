#pragma once

#include <functional>
#include <WinSock2.h>
#include <string>
#include <vector>
#include <memory>
#ifndef _M_CEE
#include <future>
#include <thread>
#endif

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
		ClientChannel();
		ClientChannel(std::string serverAddress, int port);

		~ClientChannel();

		void Listen();

		void Login(std::string user, std::string password);

		ServerPlayerInfo GetPlayerInfo();
		void GetEntities(std::string screen_name);

	private:

		bool stop_listen;
		SOCKET channel_socket;

		sockaddr_in server_addr;
		int server_addr_size;

#ifndef _M_CEE
		std::thread listen_thread;
		// TODO: remove promise, incompatible with clr.
		std::shared_ptr<std::promise<s_launcher_login_response>> get_login_response;
#endif
	};
}
