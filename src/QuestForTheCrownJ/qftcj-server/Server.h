#pragma once
#include <WinSock2.h>

#include "sqlite3.h"
#include "ServerChannel.h"
#include "ServerStructs.h"
#include "Game.h"
#include "Level.h"

namespace qfcserver {
	class Server
		: public qfcbase::Game
	{
		public:
			Server(int port);
			~Server();

			void Run();
			void Update(double delta);

			void GoToNeighbour(std::shared_ptr<qfcbase::Entity> entity, qfcbase::Direction direction);
			void StartConfront(std::shared_ptr<qfcbase::Entity> e1, std::shared_ptr<qfcbase::Entity> e2);
		private:
			sqlite3* db;
			qfcnet::ServerChannel channel;
			void UpdateLoop();

			//map scenes = no MainGame

			LauncherLoginResponse HandleLoginInfo(const LauncherLoginInfo& login_info);
			std::shared_ptr<qfcbase::Entity> CreatePlayerEntity(std::string map_name, int x, int y);
			std::shared_ptr<ServerPlayerInfo> HandleRequestPlayerInfo(const RequestPlayerInfo& data);
			std::map<std::string, LoggedUser> logged_users;
	};
}
