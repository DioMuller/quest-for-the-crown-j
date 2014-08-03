#pragma once
#include <WinSock2.h>

#include "sqlite3.h"
#include "ServerChannel.h"
#include "ServerStructs.h"
#include "Game.h"
#include "ServerLevel.h"
#include "Hero.h"
#include "ServerRuntimeStructs.h"
#include "EntityFactory.h"

namespace qfcserver {
	class Server :
		public qfcbase::Game,
		public qfcbase::EntityFactory
	{
		public:
			Server(int port);
			~Server();

			void Run();
			void Update(double delta);

			void GoToNeighbour(std::shared_ptr<qfcbase::Entity> entity, qfcbase::Direction direction);
			void StartConfront(std::shared_ptr<qfcbase::Entity> e1, std::shared_ptr<qfcbase::Entity> e2);
			void UnstackScene(std::shared_ptr<qfcbase::Entity> entity);
			std::shared_ptr<qfcbase::Entity> GenerateEntity(std::weak_ptr<qfcbase::Scene> scene, std::string name, tmx::MapObject object);
		private:
			sqlite3* db;
			std::shared_ptr<qfcnet::ServerChannel> channel;
			void UpdateLoop();

			LoggedUser GetUserInfo(const qfcbase::Entity& entity);

			LauncherLoginResponse HandleLoginInfo(const LauncherLoginInfo& login_info, std::shared_ptr<sockaddr_in> sender, int sender_size);
			std::shared_ptr<qfcbase::Hero> CreatePlayerEntity(std::string map_name, std::string animation, float x, float y);

			void SetEntityPosition(std::shared_ptr<qfcbase::Entity> player_entity, std::string map_file, std::string animation, float x, float y);

			int next_player_id;
			void HandleRequestPlayerInfo(LoggedUser& user);
			std::string GetUserAuthCode(std::shared_ptr<qfcbase::Entity> entity);
			void SendEntitiesToPlayer(LoggedUser user);
			std::map<std::string, LoggedUser> logged_users;
			std::map<std::string, std::shared_ptr<ServerLevel>> loaded_levels;

			bool IsPlayer(const qfcbase::Entity& entity);
			std::string GetMapFile(std::string mapName);
			std::string GetMapName(std::string mapFile);
			void SendEntityToUsers(int id, EntityType type, int x, int y);
	};
}
