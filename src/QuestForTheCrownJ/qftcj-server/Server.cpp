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

	channel.handleLoginInfo = [this](const LauncherLoginInfo& info){
		return HandleLoginInfo(info);
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
	std::stringstream logBuilder;
	logBuilder << "Login: " << login_info.login << " (" << login_info.hashedPassword << ")";
	Log::Message(logBuilder.str());

	std::stringstream getUserSql;
	getUserSql << "SELECT id, player_id, password FROM users WHERE login=\'" << login_info.login << "\'";

	s_launcher_login_response resp;
	ZeroMemory(&resp, sizeof(resp));
	resp.authenticated = false;
	bool user_exists = false;

	sqlite3_exec(db, getUserSql.str(), [&](int argc, char** argv, char** azColName) {
		user_exists = true;
		if (strcmp(login_info.hashedPassword, argv[2]) == 0)
			resp.authenticated = true;
	});

	if (!user_exists) {
		std::stringstream createPlayerSql;
		createPlayerSql << "INSERT INTO players(map, x, y) VALUES(1, 320, 256)";
		sqlite3_exec(db, createPlayerSql.str(), nullptr);
		auto playerId = sqlite3_last_insert_rowid(db);

		std::stringstream createUserSql;
		createUserSql << "INSERT INTO users(login, password, player_id) VALUES(\'" << login_info.login << "\', \'" << login_info.hashedPassword << "\', " << playerId << ")";
		sqlite3_exec(db, createUserSql.str(), nullptr);
		resp.authenticated = true;
	}

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
	}
	else
		Log::Debug("Invalid username / password");

	return resp;
}
