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

using namespace qfcserver;
using namespace qfcbase;

Server::Server(int port)
	: channel(port)
{
	if (sqlite3_open("database.sqlite", &db))
		throw std::exception(("Can't open database.sqlite: " + (std::string)sqlite3_errmsg(db)).c_str());

	channel.handleLoginInfo = [this](const LauncherLoginInfo& info) {
		return HandleLoginInfo(info);
	};
	channel.handleRequestPlayer = [this](const RequestPlayerInfo& data) {
		return HandleRequestPlayerInfo(data);
	};
}


Server::~Server()
{
}


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

		// foreach player screen
		//screen->Update(delta);

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

LauncherLoginResponse Server::HandleLoginInfo(const LauncherLoginInfo& login_info)
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
	std::shared_ptr<Entity> playerEntity;

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

		// Buscando informações do usuário
		sqlite3_exec(db, std::string("SELECT maps.name, x, y FROM players JOIN maps ON maps.id=players.map WHERE players.id = ") + argv[1], [&](int c, char** v, char** n) {
			playerEntity = CreatePlayerEntity(std::string(v[0]), std::atoi(v[1]), std::atoi(v[2]));
			mapName = std::string(v[0]);
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

		mapName = "Overworld01";
		playerEntity = CreatePlayerEntity(mapName, 320, 256);
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
		strcpy_s(resp.authKey, sizeof(resp.authKey), userAuthCode.final().c_str());
		Log::Debug("User authenticated: " + userAuthCode.final());

		logged.player_id = playerId;
		logged.user_id = userId;
		logged.game_entity = playerEntity;
		logged.map_name = mapName;

		logged_users[resp.authKey] = logged;
	}
	else
		Log::Debug("Invalid username / password");

	return resp;
}

std::shared_ptr<qfcbase::Entity> qfcserver::Server::CreatePlayerEntity(std::string map_name, int x, int y)
{
	// TODO: Create player entity
	return nullptr;
}

std::shared_ptr<ServerPlayerInfo> qfcserver::Server::HandleRequestPlayerInfo(const RequestPlayerInfo& data)
{
	if (logged_users.count(data.clientHeader.authKey) == 0)
	{
		Log::Error(std::string("Request from invalid user: ") + data.clientHeader.authKey);
		return nullptr;
	}
	auto user = logged_users[data.clientHeader.authKey];
	auto resp = std::make_shared<ServerPlayerInfo>();
	// TODO: Fill struct data
	//resp.entity.type
	//resp.entity.id
	strcpy_s(resp->map_name, sizeof(resp->map_name), user.map_name.c_str());
	//resp.position.x
	//resp.position.y
	return resp;
}
