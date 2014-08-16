#include <chrono>
#include <thread>
#include <iostream>
#include <memory>

#include "Window.h"
#include "MainGame.h"
#include "MultiplayerGame.h"
#include "LevelLoader.h"
#include "GameEntityFactory.h"
#include "Controllable.h"
#include "KeyboardInput.h"
#include "Log.h"

#define HIDE_CONSOLE 0

#if HIDE_CONSOLE
#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif

std::shared_ptr<qfcgame::MainGame> CreateSinglePlayer();
std::shared_ptr<qfcgame::MainGame> CreateMultiplayer(std::string server_addr, std::string auth_token);

int main(int argc, char* argv[])
{
	// Hides Console, if option is enabled.
	#if HIDE_CONSOLE
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
	#endif

	// Disables SFML errors messages, since we don't want all the "Could not load image" errors.
	std::cerr.rdbuf(0);
	sf::err().rdbuf(0);

	auto window = std::make_shared<qfcbase::Window>(sf::Vector2i(800, 600), "Quest for the Crown J");
	std::shared_ptr<qfcgame::MainGame> game;

	// Loads Level INFO.
	qfcbase::LevelLoader::LoadLevels("Content/maps/QuestForTheCrown.maps");

	if (argc <= 1)
	{
		game = CreateSinglePlayer();
	}
	else
	{
		std::string auth_token = argv[1];
		std::string server_ip = argc > 2? argv[2] : "127.0.0.1";

		game = CreateMultiplayer(server_ip, auth_token);
	}

	window->Run(game);
	return 0;
}

std::shared_ptr<qfcgame::MainGame> CreateSinglePlayer()
{
	qfcbase::LevelLoader::SetFactory(std::make_shared<qfcbase::GameEntityFactory>());
	qfcbase::Log::SetVerboseLevel(LOG_ALL);

	auto game = std::make_shared<qfcgame::MainGame>();
	auto scene = qfcbase::LevelLoader::LoadMap(game, 1, "Content/maps/Overworld01.tmx");
	game->LoadScene(scene, false);
	return game;
}

std::shared_ptr<qfcgame::MainGame> CreateMultiplayer(std::string server_addr, std::string auth_token)
{
	auto game = std::make_shared<qfcgame::MultiplayerGame>();
	game->Connect(server_addr, auth_token);
	game->RefreshSceneFromServer();
	return game;
}
