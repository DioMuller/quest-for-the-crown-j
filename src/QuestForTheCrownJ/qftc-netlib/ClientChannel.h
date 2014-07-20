#pragma once
#include <WinSock2.h>
#include <string>
#include <vector>
#include "StructBase.h"

namespace qfcnet
{
	class ClientChannel
	{
	public:
		ClientChannel(std::string serverAddress, int port);
		~ClientChannel();

		std::string Login(std::string user, std::string password);
		PlayerInfo GetPlayerInfo(std::string auth_token);
		std::vector<EntityInfo> GetEntities(std::string screen_name);

	private:
		SOCKET server_socket;
	};
}
