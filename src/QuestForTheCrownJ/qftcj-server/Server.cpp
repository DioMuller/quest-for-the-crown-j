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

using namespace qfcserver;
using namespace qfcbase;

#pragma region Constructors
Server::Server(int port)
	: channel(port), next_player_id(0)
{
	qfcbase::LevelLoader::SetFactory(new ServerEntityFactory());
	qfcbase::LevelLoader::LoadLevels("Content/maps/QuestForTheCrown.maps");

	if (sqlite3_open("database.sqlite", &db))
		throw std::exception(("Can't open database.sqlite: " + (std::string)sqlite3_errmsg(db)).c_str());

	channel.handleLoginInfo = [this](const LauncherLoginInfo& info, sockaddr_in sender, int sender_size) {
		return HandleLoginInfo(info, sender, sender_size);
	};
	channel.handleRequestPlayer = [this](const ClientRequestPlayerInfo& data) {
		return HandleRequestPlayerInfo(data);
	};
	channel.handlePlayerPosition = [this](const ClientSendPlayerPosition& data) {
		auto clientData = logged_users[data.header.authKey];
		SetEntityPosition(clientData.game_entity, clientData.map_file, data.position.x, data.position.y);
	};
	channel.handlePlayerFullPosition = [this](const ClientSendPlayerFullPosition& data) {
		auto clientData = logged_users[data.header.authKey];
		SetEntityPosition(clientData.game_entity, GetMapFile(data.player.map_name), data.player.position.x, data.player.position.y);
	};
	channel.handleRequestEntities = [this](const ClientRequestEntities& data) {
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
	auto neighborMap = LevelCollection::GetNeighbour(level->Id(), direction);

	// TODO: calc x, y based on direction
	auto entityPosition = entity->Sprite->Position;

	SetEntityPosition(entity, neighborMap, entityPosition.x, entityPosition.y);
}


void qfcserver::Server::UnstackScene(std::shared_ptr<qfcbase::Entity> entity)
{

}
#pragma endregion

#pragma region Network
LauncherLoginResponse Server::HandleLoginInfo(const LauncherLoginInfo& login_info, sockaddr_in sender, int sender_size)
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
	std::string mapName;
	long long userId;
	long long playerId;
	std::shared_ptr<Hero> playerEntity;

	std::stringstream getUserSql;
	getUserSql << "SELECT id, player_id, password FROM users WHERE login=\'" << login_info.login << "\'";

	// Buscando usu�rio j� existente
	sqlite3_exec(db, getUserSql.str(), [&](int argc, char** argv, char** azColName) {
		user_exists = true;

		// Validando senha
		if (strcmp(login_info.hashedPassword, argv[2]) != 0)
			return;

		userId = std::stoll(argv[0]);
		playerId = std::stoll(argv[1]);

		// Buscando informa��es do usu�rio
		sqlite3_exec(db, std::string("SELECT maps.name, x, y FROM players JOIN maps ON maps.id=players.map WHERE players.id = ") + argv[1], [&](int c, char** v, char** n) {
			playerEntity = CreatePlayerEntity(std::string(v[0]), std::atoi(v[1]), std::atoi(v[2]));
			mapName = std::string(v[0]);
		});

		resp.authenticated = true;
	});

	// Caso n�o exista, cria um usu�rio padr�o
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

		mapName = "Overworld01";
		playerEntity = CreatePlayerEntity(mapName, 320, 256);
	}

	// Caso autenticado, gera auth_code e guarda usu�rio como logado.
	if (resp.authenticated)
	{
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::stringstream hashSeed;
		hashSeed << login_info.login << login_info.hashedPassword << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H-%M-%S");

		SHA1 userAuthCode;
		userAuthCode.update(hashSeed);
		strcpy_s(resp.authKey, sizeof(resp.authKey), userAuthCode.final().c_str());
		Log::Debug("User authenticated: " + userAuthCode.final());

		logged.player_id = playerId;
		logged.user_id = userId;
		logged.game_entity = playerEntity;
		logged.map_file = GetMapFile (mapName);
		logged.address = sender;
		logged.address_size = sender_size;
		logged_users[resp.authKey] = logged;
	}
	else
		Log::Debug("Invalid username / password");

	return resp;
}

std::shared_ptr<ServerResponsePlayerInfo> qfcserver::Server::HandleRequestPlayerInfo(const ClientRequestPlayerInfo& data)
{
	if (logged_users.count(data.header.authKey) == 0)
	{
		Log::Error(std::string("Request from invalid user: ") + data.header.authKey);
		return nullptr;
	}
	auto user = logged_users[data.header.authKey];
	auto resp = std::make_shared<ServerResponsePlayerInfo>();
	auto pos = user.game_entity->Sprite->Position;
	// TODO: Player entity types
	resp->entity.type = EntityType::ENTITY_HERO;
	resp->entity.entityId = user.game_entity->Id;
	strcpy_s(resp->player.map_name, sizeof(resp->player.map_name), GetMapName(user.map_file).c_str());
	resp->player.position.x = static_cast<int>(pos.x);
	resp->player.position.y = static_cast<int>(pos.y);

	SendEntitiesToPlayer(user);
	return resp;
}

void qfcserver::Server::SetEntityPosition(std::shared_ptr<qfcbase::Entity> entity, std::string map_file, float x, float y)
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
	std::shared_ptr<ServerLevel> level;
	if (loaded_levels.count(map_file) == 0)
	{
		level = std::make_shared<ServerLevel>(getptr(), 1);
		level->LoadMap(map_file);

		loaded_levels[map_file] = level;
	}
	else {
		level = loaded_levels[map_file];
	}

	auto code = GetUserAuthCode(entity);
	if (code.size() > 0)
	{
		auto info = logged_users[code];
		info.map_file = map_file;
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
			if (ent == user.game_entity)
				continue;

			ServerSendEntity info;
			if (std::dynamic_pointer_cast<Hero>(ent))
				info.entity.type = ENTITY_HERO;
			else if (std::dynamic_pointer_cast<Slime>(ent))
				info.entity.type = ENTITY_SLIME;
			else
				continue;
			info.header.type = SERVER_ENTITY_INFO;
			info.position.x = ent->Sprite->Position.x;
			info.position.y = ent->Sprite->Position.y;
			info.entity.entityId = ent->Id;
			channel.Send(info, user.address, user.address_size);
		}
	}
	//});
}

void Server::SendEntityToUsers(int id, EntityType type, int x, int y)
{
	ServerSendEntity data;
	data.entity.entityId = id;
	data.entity.type = type;
	data.position.x = x;
	data.position.y = y;

	for (auto user : logged_users)
		channel.Send(data, user.second.address, user.second.address_size);
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

std::shared_ptr<Hero> qfcserver::Server::CreatePlayerEntity(std::string map_name, float x, float y)
{
	auto player_entity = std::make_shared<Hero>();
	player_entity->Id = next_player_id++;
	// TODO: Add Tracking behaviors

	auto map_file = (std::string)"Content/maps/" + (std::string)map_name + (std::string)".tmx";
	SetEntityPosition(player_entity, map_file, x, y);

	return player_entity;
}

bool qfcserver::Server::IsPlayer(const Entity& entity)
{
	return true;
}

#pragma endregion
