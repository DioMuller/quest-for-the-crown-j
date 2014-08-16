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
#include "Scene.h"
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
		if (!IsLogged(header.authKey))
			return;
		auto user = logged_users[header.authKey];
		user.address = sender;
		user.address_size = sender_size;
		logged_users[header.authKey] = user;
		user.away_time = 0;
	};
	channel->handleLoginInfo = [this](const LauncherLoginInfo& info, std::shared_ptr<sockaddr_in> sender, int sender_size) {
		return HandleLoginInfo(info, sender, sender_size);
	};
	channel->handleRequestPlayer = [this](const ClientRequestPlayerInfo& data) {
		auto user = logged_users[data.header.authKey];
		HandleRequestPlayerInfo(user);
	};
	channel->handlePlayerPosition = [this](const ClientSendPlayerPosition& data) {
		if (!IsLogged(data.header.authKey))
			return;

		auto user = logged_users[data.header.authKey];
		if (!user.game_entity)
			return;

		SetEntityPosition(user.game_entity, data.location.map_id, NetHelper::DecodeAnimation(data.view.animation), data.location.position.x, data.location.position.y);

		ServerSendEntity broadcast_data;
		broadcast_data.entity.view.animation = NetHelper::EncodeAnimation(user.game_entity->Sprite->CurrentAnimation);
		broadcast_data.entity.info.id = user.game_entity->Id;
		broadcast_data.entity.info.type = ServerHelper::GetEntityType(user.game_entity);
		broadcast_data.entity.info.category = NetHelper::EncodeCategory(user.game_entity->category);
		broadcast_data.entity.location.map_id = std::dynamic_pointer_cast<Level>(user.game_entity->Scene().lock())->Id();
		broadcast_data.entity.location.position = user.game_entity->Sprite->Position;

		for (auto other : logged_users)
		{
			if (other.first != (std::string)data.header.authKey)
				channel->Send(broadcast_data, other.second.address, other.second.address_size);
		}
	};
	channel->handleRequestEntities = [this](const ClientRequestEntities& data) {
		auto user = logged_users[data.header.authKey];
		SendEntitiesToPlayer(user);
	};
	channel->handlePlayerDisconnect = [this](const ClientSendDisconnect& data) {
		std::string auth_key = data.header.authKey;
		DisconnectPlayer(auth_key);

	};
	channel->handleConnectionClose = [this](std::shared_ptr<sockaddr_in> addr) {
		std::string user_code;
		for (auto usr : logged_users)
		{
			if (usr.second.address == addr)
			{
				user_code = usr.first;
				break;
			}
		}

		if (!user_code.empty())
			DisconnectPlayer(user_code);
	};

	// Battle

	channel->handleCharacterRequestNextTurn = [this](const ClientCharacterBattleNextTurn data)
	{
		Log::Message("Received Next Turn Request from Client");
	};

	channel->handleCharacterCommand = [this](const ClientCharacterBattleCommand data)
	{
		Log::Message("Received Command " + std::to_string(data.command) + " from Client.");
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

#pragma region Game
void Server::Update(double delta)
{
	CheckPlayersTimeout(delta);

	for (auto& battle : ongoing_battles)
		battle->Update(delta);

	for (auto& level : loaded_levels)
		level.second->Update(delta);
}

void Server::StartConfront(std::shared_ptr<qfcbase::Entity> e1, std::shared_ptr<qfcbase::Entity> e2)
{
	std::lock_guard<std::mutex> lock(mtx_battles);
	Log::Debug((std::string)"Battle: " + std::to_string(e1->Id) + " VS " + std::to_string(e2->Id));

	std::shared_ptr<ServerBattle> battle;
	if (entity_battles.count(e1->Id) > 0) {
		battle = entity_battles[e1->Id];
		if (entity_battles.count(e2->Id) <= 0)
			AddToBattle(battle, e2);
	}
	else if (entity_battles.count(e2->Id) > 0) {
		battle = entity_battles[e2->Id];
		if (entity_battles.count(e1->Id) <= 0)
			AddToBattle(battle, e1);
	}
	else {
		battle = std::make_shared<ServerBattle>(getptr());
		AddToBattle(battle, e1);
		AddToBattle(battle, e2);

		ongoing_battles.push_back(battle);
		entity_battles[e1->Id] = battle;
		entity_battles[e2->Id] = battle;
	}

	e2->Scene().lock()->RemoveEntity(e2);
}

void Server::AddToBattle(std::shared_ptr<ServerBattle> battle, std::shared_ptr<Entity> entity)
{
	if (entity->category == "GoodGuy")
		battle->PlayerJoin(entity);
	else
		battle->AddMonster(entity);
}

void Server::GoToNeighbour(std::shared_ptr<qfcbase::Entity> entity, qfcbase::Direction direction)
{
	auto level = std::dynamic_pointer_cast<Level>(entity->scene.lock());
	auto neighbor = LevelCollection::GetNeighbour(level->Id(), direction);
	if (!neighbor)
		return;

	// TODO: calc x, y based on direction?
	auto entityPosition = entity->Sprite->Position;
	SetEntityPosition(entity, neighbor->levelId, entity->Sprite->CurrentAnimation, entityPosition.x, entityPosition.y);
}

void Server::UnstackScene(std::shared_ptr<qfcbase::Entity> entity)
{
	std::lock_guard<std::mutex> lock(mtx_battles);
	if (entity_battles.count(entity->Id) <= 0)
		return;

	auto battle = entity_battles[entity->Id];
	auto found = battle->GetEntities([entity](std::shared_ptr<Entity> e) {
		auto bEnt = std::dynamic_pointer_cast<BattleEntity>(e);
		return bEnt->GetParent() == entity;
	});
	for (auto bEnt : found)
		battle->RemoveEntity(bEnt);
	if (battle->IsEmpty())
	{
		entity_battles.erase(entity->Id);
		ongoing_battles.erase(std::remove(ongoing_battles.begin(), ongoing_battles.end(), battle), ongoing_battles.end());
	}
}
#pragma endregion

#pragma region Network
LauncherLoginResponse Server::HandleLoginInfo(const LauncherLoginInfo& login_info, std::shared_ptr<sockaddr_in> sender, int sender_size)
{
	// Log
	std::stringstream logBuilder;
	logBuilder << "Login: " << login_info.login;
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
	db_exec(getUserSql.str(), [&](int argc, char** argv, char** azColName) {
		user_exists = true;

		// Validando senha
		if (strcmp(login_info.hashedPassword, argv[2]) != 0)
			return;

		userId = std::stoll(argv[0]);
		playerId = std::stoll(argv[1]);

		// TODO: Remove unused maps table?
		// Buscando informações do usuário
		db_exec(std::string("SELECT map, x, y FROM players WHERE players.id = ") + argv[1], [&](int c, char** v, char** n) {
			playerEntity = CreatePlayerEntity(std::atoi(v[0]), "stopped_down", (float) std::atoi(v[1]), (float) std::atoi(v[2]));
			map_id = std::atoi(v[0]);
		});

		resp.authenticated = true;
	});

	// Caso não exista, cria um usuário padrão
	if (!user_exists) {
		map_id = 5;
		int initialPosX = rand() % 500 + 300;
		int initialPosY = rand() % 500 + 300;

		std::stringstream createPlayerSql;
		createPlayerSql << "INSERT INTO players(map, x, y) VALUES(" << map_id << ", " << initialPosX << ", " << initialPosY << ")";
		db_exec(createPlayerSql.str(), nullptr);
		playerId = sqlite3_last_insert_rowid(db);

		std::stringstream createUserSql;
		createUserSql << "INSERT INTO users(login, password, player_id) VALUES(\'" << login_info.login << "\', \'" << login_info.hashedPassword << "\', " << playerId << ")";
		db_exec(createUserSql.str(), nullptr);
		userId = sqlite3_last_insert_rowid(db);
		resp.authenticated = true;

		playerEntity = CreatePlayerEntity(map_id, "stopped_down", (float)initialPosX, (float)initialPosY);
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

void Server::HandleRequestPlayerInfo(LoggedUser& user)
{
	if (!user.game_entity)
		return;

	ServerResponsePlayerInfo resp;
	auto pos = user.game_entity->Sprite->Position;
	resp.entity.info.type = ServerHelper::GetEntityType(user.game_entity);
	resp.entity.info.id = user.game_entity->Id;
	resp.entity.info.category = NetHelper::EncodeCategory(user.game_entity->category);
	resp.entity.location.map_id = user.map_id;
	resp.entity.location.position = pos;

	channel->Send(resp, user.address, user.address_size);
}

void Server::SetEntityPosition(std::shared_ptr<qfcbase::Entity> entity, int map_id, std::string animation, float x, float y)
{
	auto find_entity = [entity](const std::shared_ptr<Entity>& e){ return e == entity; };

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
	auto level = LoadLevel(map_id);


	level->AddEntity(entity);

	auto code = GetUserAuthCode(entity);
	if (code.size() > 0)
	{
		auto info = logged_users[code];
		bool send_entities = info.map_id != map_id;
		info.map_id = map_id;
		logged_users[code] = info;

		if (send_entities)
			SendEntitiesToPlayer(info);
	}
}

void Server::SendEntitiesToPlayer(LoggedUser user)
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
				channel->SendEntity(level, ent, user.address, user.address_size);
			}
		}
	}
	//});
}
#pragma endregion

#pragma region EntityFactory
std::shared_ptr<Entity> Server::GenerateEntity(std::weak_ptr<Scene> scene, std::string name, tmx::MapObject object)
{
	if (name == "Player")
		return nullptr;
	auto ent = EntityFactory::GenerateEntity(scene, name, object);

	if (ent)
	{
		ent->AddBehavior<WatchPosition>([&](std::shared_ptr<Entity> e) {
			auto level = std::dynamic_pointer_cast<Level>(e->Scene().lock());
			if (level)
			{
				for (auto user : logged_users)
				{
					if (user.second.game_entity != e)
						channel->SendEntity(level, e, user.second.address, user.second.address_size);
				}
			}
		}, NET_SECONDS_PER_FRAME);
	}

	return ent;
}
#pragma endregion

#pragma region Helpers
void Server::CheckPlayersTimeout(double dt)
{
	std::vector<std::string> timeout_players;

	for (auto& user : logged_users)
	{
		user.second.away_time += dt;
		if (user.second.away_time > 10)
			timeout_players.push_back(user.first);
	}

	for (auto& user_code : timeout_players)
		DisconnectPlayer(user_code);
}

std::string Server::GetUserAuthCode(std::shared_ptr<qfcbase::Entity> entity)
{
	for (auto& user : logged_users) {
		if (user.second.game_entity == entity)
			return user.first;
	}
	return std::string();
}

std::shared_ptr<Hero> Server::CreatePlayerEntity(int map_id, std::string animation, float x, float y)
{
	auto player_entity = std::make_shared<Hero>();
	player_entity->Id = GenerateId();
	player_entity->category = "GoodGuy";
	SetEntityPosition(player_entity, map_id, animation, x, y);
	return player_entity;
}

bool Server::IsLogged(std::string authKey)
{
	return logged_users.count(authKey) > 0;
}

void Server::db_exec(std::string sql, const std::function<void(int argc, char** argv, char** azColName)>& callback)
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

void Server::DisconnectPlayer(std::string auth_key)
{
	auto user = logged_users[auth_key];
	SavePlayer(user);
	if (user.game_entity)
	{
		ServerSendEntityRemoved broadcast_data;
		broadcast_data.entity_id = user.game_entity->Id;
		for (auto usr : logged_users)
			channel->Send(broadcast_data, usr.second.address, usr.second.address_size);

		if (auto level = user.game_entity->Scene().lock())
			level->RemoveEntity(user.game_entity);

		UnstackScene(user.game_entity);
	}
	logged_users.erase(auth_key);

	Log::Debug(std::string("Client disconnected: ") + auth_key);
}

void Server::SavePlayer(LoggedUser user)
{
	auto pos = user.game_entity->Sprite->Position;

	std::ostringstream update_builder;
	update_builder << "UPDATE players set map=" << user.map_id << ", x=" << static_cast<int>(pos.x) << ", y=" << static_cast<int>(pos.y) << " ";
	update_builder << "WHERE id=" << user.player_id;

	db_exec(update_builder.str(), [&](int c, char** v, char** n) { });
}

std::shared_ptr<ServerLevel> qfcserver::Server::LoadLevel(int map_id)
{
	if (loaded_levels.count(map_id) == 0)
	{
		auto level = std::make_shared<ServerLevel>(getptr(), 1);
		level->LoadMap(LevelCollection::GetLevel(map_id)->mapFile);
		loaded_levels[map_id] = level;
	}
	return loaded_levels[map_id];
}

void qfcserver::Server::SendTurn(int turn_id, qfcbase::BattleAction command, int target_id, int additional_info, std::shared_ptr<Entity> entity)
{
	// TODO: SEND Turn data.
	for (auto usr : logged_users)
	{
		if (usr.second.game_entity->Id == entity->Id)
			channel->SendServerCommand(turn_id, command, target_id, additional_info, usr.second.address, usr.second.address_size);
	}
}

#pragma endregion
