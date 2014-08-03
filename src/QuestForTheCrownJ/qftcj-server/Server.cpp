#include "Server.h"

#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <future>

#include "Log.h"
#include "StructBase.h"
#include "AuthStructs.h"
#include "ClientStructs.h"
#include "Definitions.h"
#include "SHA1.h"
#include "LevelLoader.h"
#include "Slime.h"
#include "ServerEntityFactory.h"
#include "ServerHelper.h"
#include "WatchPosition.h"
#include "NetDefinitions.h"
#include "NetHelper.h"

using namespace qfcserver;
using namespace qfcbase;
using namespace qfcnet;

#pragma region Constructors
Server::Server(int port)
{
	channel = std::make_shared<ServerChannel>(port);

	if (sqlite3_open("database.sqlite", &db))
		throw std::exception(("Can't open database.sqlite: " + (std::string)sqlite3_errmsg(db)).c_str());

	channel->onClientMessage = [this](const ClientHeader& header, std::shared_ptr<sockaddr_in> sender, int sender_size) {
		if (logged_users.count(header.authKey) <= 0)
			return;
		auto user = logged_users[header.authKey];
		user.address = sender;
		user.address_size = sender_size;
		logged_users[header.authKey] = user;
	};
	channel->handleLoginInfo = [this](const LauncherLoginInfo& info, std::shared_ptr<sockaddr_in> sender, int sender_size) {
		return HandleLoginInfo(info, sender, sender_size);
	};
	channel->handleRequestPlayer = [this](const ClientRequestPlayerInfo& data) {
		auto user = logged_users[data.header.authKey];
		HandleRequestPlayerInfo(user);
	};
	channel->handlePlayerPosition = [this](const ClientSendPlayerPosition& data) {
		auto user = logged_users[data.header.authKey];
		SetEntityPosition(user.game_entity, data.location.map_id, NetHelper::DecodeAnimation(data.view.animation), data.location.position.x, data.location.position.y);

		ServerSendEntity resp;
		resp.entity.view.animation = NetHelper::EncodeAnimation(user.game_entity->Sprite->CurrentAnimation);
		resp.entity.info.id = user.game_entity->Id;
		resp.entity.info.type = ServerHelper::GetEntityType(user.game_entity);
		resp.entity.location.map_id = std::dynamic_pointer_cast<Level>(user.game_entity->Scene().lock())->Id();
		resp.entity.location.position = user.game_entity->Sprite->Position;

		for (auto other : logged_users)
		{
			if (other.first != (std::string)data.header.authKey)
				channel->Send(resp, other.second.address, other.second.address_size);
		}
	};
	channel->handlePlayerFullPosition = [this](const ClientSendPlayerFullPosition& data) {
		auto clientData = logged_users[data.header.authKey];
		SetEntityPosition(clientData.game_entity, data.location.map_id, NetHelper::DecodeAnimation(data.view.animation), data.location.position.x, data.location.position.y);
	};
	channel->handleRequestEntities = [this](const ClientRequestEntities& data) {
		auto user = logged_users[data.header.authKey];
		SendEntitiesToPlayer(user);
	};
}

Server::~Server()
{
}
#pragma endregion

void Server::Run()
{
	qfcbase::LevelLoader::SetFactory(std::dynamic_pointer_cast<EntityFactory>(getptr()));
	qfcbase::LevelLoader::LoadLevels("Content/maps/QuestForTheCrown.maps");
	UpdateLoop();
}

void Server::UpdateLoop()
{
	//Initial update duration = 1 frame.
	double delta = SECONDS_PER_FRAME;

	while (true)
	{
		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();

		Update(delta);

		end = std::chrono::system_clock::now();

		delta = ((double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / (1000.0 * 1000.0);

		if (delta < SECONDS_PER_FRAME)
		{
			std::this_thread::sleep_for(std::chrono::microseconds((long long)((SECONDS_PER_FRAME - delta) * (1000 * 1000))));
			delta = SECONDS_PER_FRAME;
		}
	}
}

void sqlite3_exec(sqlite3* db, std::string sql, const std::function<void(int argc, char** argv, char** azColName)>& callback)
{
	char *zErrMsg = 0;
	int res = sqlite3_exec(db, sql.c_str(), [](void* data, int argc, char** argv, char** azColName) {
		auto c = (std::function<void(int argc, char** argv, char** azColName)>*)data;
		if (c) (*c)(argc, argv, azColName);
		return 0;
	}, (void*)&callback, &zErrMsg);
	if (res != SQLITE_OK)
	{
		auto ex = std::exception(zErrMsg);
		sqlite3_free(zErrMsg);
		throw ex;
	}
}

#pragma region Game
void qfcserver::Server::Update(double delta)
{
	for (auto& level : loaded_levels)
		level.second->Update(delta);
}

void qfcserver::Server::StartConfront(std::shared_ptr<qfcbase::Entity> e1, std::shared_ptr<qfcbase::Entity> e2)
{

}

void qfcserver::Server::GoToNeighbour(std::shared_ptr<qfcbase::Entity> entity, qfcbase::Direction direction)
{
	auto level = std::dynamic_pointer_cast<Level>(entity->scene.lock());
	auto neighbor = LevelCollection::GetNeighbour(level->Id(), direction);
	if (!neighbor)
		return;

	// TODO: calc x, y based on direction?
	auto entityPosition = entity->Sprite->Position;
	SetEntityPosition(entity, neighbor->levelId, entity->Sprite->CurrentAnimation, entityPosition.x, entityPosition.y);
}


void qfcserver::Server::UnstackScene(std::shared_ptr<qfcbase::Entity> entity)
{

}
#pragma endregion

#pragma region Network
LauncherLoginResponse Server::HandleLoginInfo(const LauncherLoginInfo& login_info, std::shared_ptr<sockaddr_in> sender, int sender_size)
{
	// Log
	std::stringstream logBuilder;
	logBuilder << "Login: " << login_info.login << " (" << login_info.hashedPassword << ")";
	Log::Message(logBuilder.str());

	s_launcher_login_response resp;
	ZeroMemory(&resp, sizeof(resp));
	resp.authenticated = false;
	bool user_exists = false;

	LoggedUser logged;
	int map_id;
	long long userId;
	long long playerId;
	std::shared_ptr<Hero> playerEntity;

	std::stringstream getUserSql;
	getUserSql << "SELECT id, player_id, password FROM users WHERE login=\'" << login_info.login << "\'";

	// Buscando usuário já existente
	sqlite3_exec(db, getUserSql.str(), [&](int argc, char** argv, char** azColName) {
		user_exists = true;

		// Validando senha
		if (strcmp(login_info.hashedPassword, argv[2]) != 0)
			return;

		userId = std::stoll(argv[0]);
		playerId = std::stoll(argv[1]);

		// TODO: Remove unused maps table?
		// Buscando informações do usuário
		sqlite3_exec(db, std::string("SELECT map, x, y FROM players WHERE players.id = ") + argv[1], [&](int c, char** v, char** n) {
			playerEntity = CreatePlayerEntity(std::atoi(v[0]), "stopped_down", std::atoi(v[1]), std::atoi(v[2]));
			map_id = std::atoi(v[0]);
		});

		resp.authenticated = true;
	});

	// Caso não exista, cria um usuário padrão
	if (!user_exists) {
		std::stringstream createPlayerSql;
		createPlayerSql << "INSERT INTO players(map, x, y) VALUES(1, 320, 256)";
		sqlite3_exec(db, createPlayerSql.str(), nullptr);
		playerId = sqlite3_last_insert_rowid(db);

		std::stringstream createUserSql;
		createUserSql << "INSERT INTO users(login, password, player_id) VALUES(\'" << login_info.login << "\', \'" << login_info.hashedPassword << "\', " << playerId << ")";
		sqlite3_exec(db, createUserSql.str(), nullptr);
		userId = sqlite3_last_insert_rowid(db);
		resp.authenticated = true;

		map_id = 1;
		playerEntity = CreatePlayerEntity(map_id, "stopped_down", 320, 256);
	}

	// Caso autenticado, gera auth_code e guarda usuário como logado.
	if (resp.authenticated)
	{
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::stringstream hashSeed;
		hashSeed << login_info.login << login_info.hashedPassword << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H-%M-%S");

		SHA1 userAuthCode;
		userAuthCode.update(hashSeed);
		std::string user_auth_str = userAuthCode.final();
		strcpy_s(resp.authKey, sizeof(resp.authKey), user_auth_str.c_str());
		Log::Debug("User authenticated:\n" + user_auth_str);

		playerEntity->Id = LevelLoader::GetFactory()->GenerateId();
		logged.player_id = playerId;
		logged.user_id = userId;
		logged.game_entity = playerEntity;
		logged.map_id = map_id;
		logged.address = sender;
		logged.address_size = sender_size;
		logged_users[user_auth_str] = logged;
	}
	else
		Log::Debug("Invalid username / password");

	return resp;
}

// TODO: Remove unused? GetMap (from id/name)

void qfcserver::Server::HandleRequestPlayerInfo(LoggedUser& user)
{
	if (!user.game_entity)
		return;

	ServerResponsePlayerInfo resp;
	auto pos = user.game_entity->Sprite->Position;
	// TODO: Player entity types
	resp.entity.info.type = EntityType::ENTITY_HERO;
	resp.entity.info.id = user.game_entity->Id;
	resp.entity.location.map_id = user.map_id;
	resp.entity.location.position = pos;

	channel->Send(resp, user.address, user.address_size);
}

void qfcserver::Server::SetEntityPosition(std::shared_ptr<qfcbase::Entity> entity, int map_id, std::string animation, float x, float y)
{
	auto find_entity = [entity](const std::shared_ptr<Entity>& e){ return e == entity; };
	auto is_any_player = [&](const std::shared_ptr<Entity>& e){ return IsPlayer(*e); };

	for (auto itr = loaded_levels.begin(); itr != loaded_levels.end(); ++itr) {
		if (itr->second->GetEntities(find_entity).size() <= 0)
			continue;

		itr->second->RemoveEntity(entity);

		bool map_in_use = false;
		for (auto us : logged_users)
		{
			if (us.second.game_entity->scene.lock() == itr->second)
			{
				map_in_use = true;
				break;
			}
		}

		if (!map_in_use)
			loaded_levels.erase(itr);
		break;
	}

	entity->Sprite->Position = sf::Vector2f(x, y);
	if (animation != "")
		entity->Sprite->CurrentAnimation = animation;
	std::shared_ptr<ServerLevel> level;
	if (loaded_levels.count(map_id) == 0)
	{
		level = std::make_shared<ServerLevel>(getptr(), 1);
		level->LoadMap(LevelCollection::GetLevel(map_id)->mapFile);

		loaded_levels[map_id] = level;
	}
	else {
		level = loaded_levels[map_id];
	}

	auto code = GetUserAuthCode(entity);
	if (code.size() > 0)
	{
		auto info = logged_users[code];
		info.map_id = map_id;
		logged_users[code] = info;
	}

	level->AddEntity(entity);
}

void qfcserver::Server::SendEntitiesToPlayer(LoggedUser user)
{
	//user._thread.Post([this, user](){
	auto scene = user.game_entity->Scene().lock();
	if (scene)
	{
		for (auto ent : scene->GetEntities([](const std::shared_ptr<Entity>& e) { return true; }))
		{
			auto level = std::dynamic_pointer_cast<Level>(ent->Scene().lock());
			if (level)
			{
				EntityType type = ServerHelper::GetEntityType(ent);
				channel->SendEntity(level->Id(), ent->Id, type, ent->Sprite->Position, ent->Sprite->CurrentAnimation, user.address, user.address_size);
			}
		}
	}
	//});
}
#pragma endregion

#pragma region EntityFactory
std::shared_ptr<Entity> qfcserver::Server::GenerateEntity(std::weak_ptr<Scene> scene, std::string name, tmx::MapObject object)
{
	if (name == "Player")
		return nullptr;
	auto ent = EntityFactory::GenerateEntity(scene, name, object);

	if (ent)
	{
		ent->AddBehavior(std::make_shared<WatchPosition>(ent, [&](std::shared_ptr<Entity> e) {
			auto level = std::dynamic_pointer_cast<Level>(e->Scene().lock());
			if (level)
			{
				for (auto user : logged_users)
				{
					if (user.second.game_entity != e)
						channel->SendEntity(level->Id(), e->Id, ServerHelper::GetEntityType(e), e->Sprite->Position, e->Sprite->CurrentAnimation, user.second.address, user.second.address_size);
				}
			}
		}, NET_SECONDS_PER_FRAME));
	}

	return ent;
}
#pragma endregion

#pragma region Helpers
std::string Server::GetMapFile(std::string mapName)
{
	return (std::string)"Content/maps/" + mapName + (std::string)".tmx";
}

std::string Server::GetMapName(std::string mapFile)
{
	return mapFile.substr(13, mapFile.length() - 17); // Content/maps/ .tmx
}

std::string Server::GetUserAuthCode(std::shared_ptr<qfcbase::Entity> entity)
{
	for (auto& user : logged_users) {
		if (user.second.game_entity == entity)
			return user.first;
	}
	return std::string();
}

std::shared_ptr<Hero> qfcserver::Server::CreatePlayerEntity(int map_id, std::string animation, float x, float y)
{
	auto player_entity = std::make_shared<Hero>();
	player_entity->Id = GenerateId();
	SetEntityPosition(player_entity, map_id, animation, x, y);
	return player_entity;
}

bool qfcserver::Server::IsPlayer(const Entity& entity)
{
	return true;
}

#pragma endregion
