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
		auto user = GetUser(header.authKey);
		if (!user) return;

		user->address = sender;
		user->address_size = sender_size;
		user->away_time = 0;
	};
	channel->handleLoginInfo = [this](const LauncherLoginInfo& info, std::shared_ptr<sockaddr_in> sender, int sender_size) {
		return HandleLoginInfo(info, sender, sender_size);
	};
	channel->handleRequestPlayer = [this](const ClientRequestPlayerInfo& data) {
		auto user = GetUser(data.header.authKey);
		if (!user) return;
		HandleRequestPlayerInfo(user);
	};
	channel->handlePlayerPosition = [this](const ClientSendPlayerPosition& data) {
		auto user = GetUser(data.header.authKey);
		if (!user) return;

		auto level = std::dynamic_pointer_cast<Level>(user->game_entity->Scene().lock());
		if (!level) return;

		SetEntityPosition(user->game_entity, data.location.map_id, NetHelper::DecodeAnimation(data.view.animation), data.location.position.x, data.location.position.y);

		for (auto other : GetOtherUsers(user->game_entity))
		{
			channel->SendEntity(level, user->game_entity, other->address, other->address_size);
		}
	};
	channel->handleRequestEntities = [this](const ClientRequestEntities& data) {
		auto user = GetUser(data.header.authKey);
		if (!user) return;
		SendEntitiesToPlayer(user);
	};
	channel->handlePlayerDisconnect = [this](const ClientSendDisconnect& data) {
		auto user = GetUser(data.header.authKey);
		if (!user) return;
		DisconnectPlayer(user);
	};
	channel->handleConnectionClose = [this](std::shared_ptr<sockaddr_in> addr) {
		auto user = GetUser(addr);
		if (!user) return;
		DisconnectPlayer(user);
	};

	// Battle

	channel->handleCharacterRequestNextTurn = [this](const ClientCharacterBattleNextTurn data)
	{
		auto user = GetUser(data.header.authKey);
		if (!user) {
			Log::Message("Received Turn Request from unknown client");
			return;
		}

		Log::Debug("Received request for battle turn (" + std::to_string(data.lastTurn) + ") from client");

		auto battle = entity_battles[user->game_entity->Id];

		if (battle)
		{
			battle->SendTurnToClients(data.lastTurn);
		}
		else
		{
			Log::Error("User was not found in a battle. User Entity Id: " + std::to_string(user->game_entity->Id));
		}
	};

	channel->handleCharacterCommand = [this](const ClientCharacterBattleCommand data)
	{
		Log::Message("Received Command " + std::to_string(data.command) + " from Client on turn " + std::to_string(data.turn_id) + ".");
		auto user = GetUser(data.header.authKey);
		if (!user) return;

		auto battle = entity_battles[user->game_entity->Id];

		if (battle)
		{
			battle->ReceiveTurn(data.turn_id, data.command, data.additional_info, data.entity_id, data.target_id);
		}
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
	std::lock_guard<std::recursive_mutex> lockBattles(mtx_battles);

	CheckPlayersTimeout(delta);

	auto battleIterator = std::begin(ongoing_battles);
	while (battleIterator != std::end(ongoing_battles)) {
		(*battleIterator)->Update(delta);

		if ((*battleIterator)->IsEmpty())
			battleIterator = ongoing_battles.erase(battleIterator);
		else
			++battleIterator;
	}

	for (auto& level : loaded_levels)
		level.second->Update(delta);
}

void Server::StartConfront(std::shared_ptr<qfcbase::Entity> e1, std::shared_ptr<qfcbase::Entity> e2)
{
	auto user = GetUser(e1);
	if (!user)
		return;

	// TODO: Uncomment messages.
	Log::Debug((std::string)"Battle: " + std::to_string(e1->Id) + " VS " + std::to_string(e2->Id));

	std::shared_ptr<ServerBattle> battle;
	if (entity_battles.count(e1->Id) > 0 && entity_battles[e1->Id] != nullptr) {
		/*battle = entity_battles[e1->Id];
		if (entity_battles.count(e2->Id) <= 0)
		{
		AddToBattle(battle, e2);
		e2->Scene().lock()->RemoveEntity(e2);
		throw std::logic_error("Server currently does not support multiple entities in one battle!");
		}*/
		return;
	}
	if (entity_battles.count(e2->Id) > 0 && entity_battles[e2->Id] != nullptr) {
		/*battle = entity_battles[e2->Id];
		if (entity_battles.count(e1->Id) <= 0)
		{
		AddToBattle(battle, e1);
		// TODO: Notify player
		throw std::logic_error("Server currently does not support multiple entities in one battle!");
		}*/
		return;
	}

	battle = std::make_shared<ServerBattle>(getptr());
	AddToBattle(battle, e1);
	AddToBattle(battle, e2);

	ServerBattleStart data;
	data.entity.category = NetHelper::EncodeCategory(e2->category);
	data.entity.id = e2->Id;
	data.entity.type = ServerHelper::GetEntityType(e2);
	//data.battle_id = ?;
	channel->Send(data, user->address, user->address_size);

	ongoing_battles.push_back(battle);
	entity_battles[e1->Id] = battle;
	entity_battles[e2->Id] = battle;

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
	if (entity_battles.count(entity->Id) <= 0)
		return;

	auto battle = entity_battles[entity->Id];
	auto found = battle->GetEntities([entity](std::shared_ptr<Entity> e) {
		auto bEnt = std::dynamic_pointer_cast<BattleEntity>(e);
		return bEnt->GetParent() == entity;
	});
	for (auto bEnt : found)
		battle->RemoveEntity(bEnt);

	if (entity_battles.count(entity->Id) > 0)
		entity_battles.erase(entity->Id);
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

	auto logged = std::make_shared<LoggedUser>();
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

		// Buscando informações do usuário
		db_exec(std::string("SELECT map, x, y, gold, potions FROM players WHERE players.id = ") + argv[1], [&](int c, char** v, char** n) {
			playerEntity = CreatePlayerEntity(login_info.login, std::atoi(v[0]), "stopped_down", (float)std::atoi(v[1]), (float)std::atoi(v[2]));
			playerEntity->items.gold = (int)std::atoi(v[3]);
			playerEntity->items.potions = (int)std::atoi(v[4]);
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

		playerEntity = CreatePlayerEntity(login_info.login, map_id, "stopped_down", (float)initialPosX, (float)initialPosY);
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
		logged->auth_key = user_auth_str;
		logged->player_id = playerId;
		logged->user_id = userId;
		logged->game_entity = playerEntity;
		logged->map_id = map_id;
		logged->address = sender;
		logged->address_size = sender_size;
		std::lock_guard<std::recursive_mutex> lock(mtx_users);
		logged_users[user_auth_str] = logged;
	}
	else
		Log::Debug("Invalid username / password");

	return resp;
}

void Server::HandleRequestPlayerInfo(std::shared_ptr<LoggedUser> user)
{
	ServerResponsePlayerInfo resp;
	auto pos = user->game_entity->Sprite->Position;
	resp.entity.info.type = ServerHelper::GetEntityType(user->game_entity);
	resp.entity.info.id = user->game_entity->Id;
	resp.entity.info.category = NetHelper::EncodeCategory(user->game_entity->category);
	resp.entity.location.map_id = user->map_id;
	resp.entity.location.position = pos;
	resp.items = user->game_entity->items;

	channel->Send(resp, user->address, user->address_size);
}

void Server::SetEntityPosition(std::shared_ptr<qfcbase::Entity> entity, int map_id, std::string animation, float x, float y)
{
	auto find_entity = [entity](const std::shared_ptr<Entity>& e){ return e == entity; };

	for (auto itr = loaded_levels.begin(); itr != loaded_levels.end(); ++itr) {
		if (itr->second->GetEntities(find_entity).size() <= 0)
			continue;

		itr->second->RemoveEntity(entity);

		bool map_in_use = false;
		for (auto us : GetUsers([&](std::shared_ptr<LoggedUser> u) {
			return u->game_entity->scene.lock() == itr->second;
		}))
		{
			map_in_use = true;
			break;
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

	auto info = GetUser(entity);
	if (info)
	{
		bool send_entities = info->map_id != map_id;
		info->map_id = map_id;

		if (send_entities)
			SendEntitiesToPlayer(info);
	}
}

void Server::SendEntitiesToPlayer(std::shared_ptr<LoggedUser> user)
{
	//user._thread.Post([this, user](){
	auto scene = user->game_entity->Scene().lock();
	if (scene)
	{
		for (auto ent : scene->GetEntities([](const std::shared_ptr<Entity>& e) { return true; }))
		{
			auto level = std::dynamic_pointer_cast<Level>(ent->Scene().lock());
			if (level)
			{
				EntityType type = ServerHelper::GetEntityType(ent);
				channel->SendEntity(level, ent, user->address, user->address_size);
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
				for (auto user : GetOtherUsers(e))
				{
					if (user->game_entity != e)
						channel->SendEntity(level, e, user->address, user->address_size);
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
	std::vector<std::shared_ptr<LoggedUser>> timeout_players;

	for (auto& user : GetUsers([&](std::shared_ptr<LoggedUser> user) { return true; }))
	{
		user->away_time += dt;
		if (user->away_time > 5 * 60)
			timeout_players.push_back(user);
	}

	for (auto& user : timeout_players)
		DisconnectPlayer(user);
}

std::shared_ptr<LoggedUser> Server::GetUser(std::string authKey)
{
	std::lock_guard<std::recursive_mutex> lock(mtx_users);
	if (logged_users.count(authKey) > 0)
		return logged_users[authKey];
	return nullptr;
}

std::shared_ptr<LoggedUser> Server::GetUser(std::shared_ptr<sockaddr_in> address)
{
	std::lock_guard<std::recursive_mutex> lock(mtx_users);
	for (auto usr : logged_users)
		if (usr.second->address == address)
			return usr.second;
	return nullptr;
}

std::shared_ptr<LoggedUser> Server::GetUser(std::shared_ptr<qfcbase::Entity> entity)
{
	std::lock_guard<std::recursive_mutex> lock(mtx_users);
	for (auto& user : logged_users) {
		if (user.second->game_entity == entity)
			return user.second;
	}
	return nullptr;
}

std::vector<std::shared_ptr<LoggedUser>> Server::GetUsers(std::function<bool(std::shared_ptr<LoggedUser>)> predicate)
{
	std::lock_guard<std::recursive_mutex> lock(mtx_users);
	std::vector<std::shared_ptr<LoggedUser>> users;
	for (auto& user : logged_users) {
		if (predicate(user.second))
			users.push_back(user.second);
	}
	return users;
}

std::vector<std::shared_ptr<LoggedUser>> Server::GetOtherUsers(std::shared_ptr<qfcbase::Entity> entity)
{
	return GetUsers([&](std::shared_ptr<LoggedUser> user) {
		return user->game_entity != entity;
	});
}


std::shared_ptr<Hero> Server::CreatePlayerEntity(std::string name, int map_id, std::string animation, float x, float y)
{
	auto player_entity = std::make_shared<Hero>();
	player_entity->Id = GenerateId();
	player_entity->category = "GoodGuy";
	player_entity->Name = name;
	SetEntityPosition(player_entity, map_id, animation, x, y);
	return player_entity;
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

void Server::DisconnectPlayer(std::shared_ptr<LoggedUser> user)
{
	std::lock_guard<std::recursive_mutex> lock(mtx_users);
	std::lock_guard<std::recursive_mutex> lockBattles(mtx_battles);
	SavePlayer(user);
	if (user->game_entity)
	{
		ServerSendEntityRemoved broadcast_data;
		broadcast_data.entity_id = user->game_entity->Id;
		for (auto usr : logged_users)
			channel->Send(broadcast_data, usr.second->address, usr.second->address_size);

		if (auto level = user->game_entity->Scene().lock())
			level->RemoveEntity(user->game_entity);

		UnstackScene(user->game_entity);
	}

	logged_users.erase(user->auth_key);
	Log::Debug(std::string("Client disconnected: ") + user->auth_key);
}

void Server::SavePlayer(std::shared_ptr<LoggedUser> user)
{
	auto pos = user->game_entity->Sprite->Position;

	std::ostringstream update_builder;
	update_builder << "UPDATE players set map=" << user->map_id << ", x=" << static_cast<int>(pos.x) << ", y=" << static_cast<int>(pos.y) << " ";
	update_builder << "WHERE id=" << user->player_id;

	db_exec(update_builder.str(), [&](int c, char** v, char** n) {});
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

void qfcserver::Server::SendTurn(int turn_id, qfcbase::BattleAction command, int entity_id, int target_id, int additional_info, std::shared_ptr<Entity> entity)
{
	auto user = GetUser(entity);
	if (!user) return;

	Log::Debug("Sending battle turn (" + std::to_string(turn_id) + ") to client");
	channel->SendServerCommand(turn_id, command, entity_id, target_id, additional_info, user->address, user->address_size);
}

void qfcserver::Server::AddToPlayer(std::shared_ptr<Entity> entity, int gold, int potions)
{
	auto user = GetUser(entity);
	if (!user) return;

	entity->items.gold += gold;
	entity->items.potions += potions;
	db_exec("UPDATE players SET gold=" + std::to_string(entity->items.gold) + (std::string)", potions=" + std::to_string(entity->items.potions) + " WHERE id=" + std::to_string(user->player_id), [=](int argc, char** argv, char** azClName) {
		Log::Debug(static_cast<std::string>("[Updated Player ") + std::to_string(entity->Id) + static_cast<std::string>(" Items]"));
		Log::Debug("Gold: " + std::to_string(entity->items.gold));
		Log::Debug("Potions: " + std::to_string(entity->items.potions));
	});
}
#pragma endregion
