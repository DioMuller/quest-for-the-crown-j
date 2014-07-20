#pragma once
#include <WinSock2.h>
#include <string>
#include <vector>
#include <memory>
#include "StructBase.h"

namespace qfcnet
{
	class ClientChannel
	{
	public:
		std::string auth_token;

	public:
		ClientChannel(std::string serverAddress, int port);
		~ClientChannel();

		void Login(std::string user, std::string password);

		PlayerInfo GetPlayerInfo();
		std::vector<EntityInfo> GetEntities(std::string screen_name);

	private:
		SOCKET server_socket;
		sockaddr_in server_addr;
		int server_addr_size;
	};
}
