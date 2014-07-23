#pragma once
#include <ppltasks.h>
#include <functional>
#include <WinSock2.h>
#include <string>
#include <vector>
#include <memory>
#include "StructBase.h"
#include <pplcancellation_token.h>
#include <thread>

namespace qfcnet
{
	class ClientChannel
	{
	public:
		std::string auth_token;
		std::function<void(EntityInfo)> onEntity;

	public:
		ClientChannel(std::string serverAddress, int port);
		~ClientChannel();

		void Login(std::string user, std::string password);

		PlayerInfo GetPlayerInfo();
		void GetEntities(std::string screen_name);

	private:
		void Listen(concurrency::cancellation_token);

		std::thread listen_thread;
		concurrency::cancellation_token_source cancellation_source;

		SOCKET server_socket;
		sockaddr_in server_addr;
		int server_addr_size;
	};
}
