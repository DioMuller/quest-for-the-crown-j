#pragma once
#include <WinSock2.h>
#include <string>
#include <vector>
#include "StructBase.h"
#include "Entity.h"
#include "Definitions.h"

namespace qfcnet
{
	class ClientChannel
	{
	public:
		ClientChannel(std::string serverAddress, int port);
		~ClientChannel();

		void Login(std::string user, std::string password);

		PlayerInfo GetPlayerInfo();
		std::vector<std::shared_ptr<qfcbase::Entity>> GetEntities(std::string screen_name);

		std::shared_ptr<qfcbase::Entity> CreateEntity(EntityInfo info);

	private:
		SOCKET server_socket;

		autoprop(public, public, std::string, auth_token, AuthToken);
	};
}
