#pragma once
#include <WinSock2.h>
#include <vector>
#include <mutex>

#include "sqlite3.h"
#include "ServerChannel.h"
#include "ServerStructs.h"
#include "Game.h"
#include "ServerLevel.h"
#include "Hero.h"
#include "ServerRuntimeStructs.h"
#include "EntityFactory.h"
#include "ServerBattle.h"

namespace qfcserver {
	class Server :
		public qfcbase::Game,
		public qfcbase::EntityFactory
	{
		public:
			Server(int port);

			void DisconnectPlayer(std::string auth_key);

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
			std::map<std::string, std::shared_ptr<LoggedUser>> logged_users;
			std::map<int, std::shared_ptr<ServerLevel>> loaded_levels;
			std::map<int/*EntityID*/, std::shared_ptr<qfcserver::ServerBattle>> entity_battles;
			std::vector<std::shared_ptr<qfcserver::ServerBattle>> ongoing_battles;
			std::mutex mtx_battles;

			void UpdateLoop();

			LauncherLoginResponse HandleLoginInfo(const LauncherLoginInfo& login_info, std::shared_ptr<sockaddr_in> sender, int sender_size);
			std::shared_ptr<qfcbase::Hero> CreatePlayerEntity(std::string name, int map_id, std::string animation, float x, float y);

			void SetEntityPosition(std::shared_ptr<qfcbase::Entity> player_entity, int map_id, std::string animation, float x, float y);

			void HandleRequestPlayerInfo(std::shared_ptr<LoggedUser> user);
			std::shared_ptr<LoggedUser> Server::GetUser(std::shared_ptr<qfcbase::Entity> entity);
			void Server::SendEntitiesToPlayer(std::shared_ptr<LoggedUser> user);

			void SendEntityToUsers(int id, EntityType type, int x, int y);
			std::shared_ptr<LoggedUser> Server::GetUser(std::string authKey);
			void db_exec(std::string sql, const std::function<void(int argc, char** argv, char** azColName)>& callback);
			void CheckPlayersTimeout(double dt);
			void SavePlayer(std::shared_ptr<LoggedUser> user);
			void AddToBattle(std::shared_ptr<ServerBattle> battle, std::shared_ptr<qfcbase::Entity> entity);
			std::shared_ptr<ServerLevel> LoadLevel(int map_id);
		public:
			void SendTurn(int turn_id, qfcbase::BattleAction command, int target_id, int additional_info, std::shared_ptr<qfcbase::Entity> entity);
	};
}
