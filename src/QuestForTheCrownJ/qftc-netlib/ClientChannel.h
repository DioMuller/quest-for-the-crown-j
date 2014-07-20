#pragma once
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

		PlayerInfo Login(std::string user, std::string password);
		std::vector<EntityInfo> GetEntities(std::string screen_name);

	private:
		SOCKET server_socket;
	};
}
