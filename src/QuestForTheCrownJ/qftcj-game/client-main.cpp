#include <chrono>
#include <thread>
#include <iostream>

#include "Window.h"
#include "MainGame.h"
#include "MultiplayerGame.h"
#include "LevelLoader.h"
#include "GameEntityFactory.h"
#include "Controllable.h"
#include "KeyboardInput.h"

std::shared_ptr<qfcgame::MainGame> CreateSinglePlayer();
std::shared_ptr<qfcgame::MainGame> CreateMultiplayer(std::string auth_token);

int main(int argc, char* argv[])
{
	// Disables SFML errors messages, since we don't want all the "Could not load image" errors.
	std::cerr.rdbuf(0);
	sf::err().rdbuf(0);

	qfcbase::Window window({ 800, 600 }, "Quest for the Crown J");
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
		game = CreateMultiplayer(auth_token);
	}

	window.Run(game);
	return 0;
}

std::shared_ptr<qfcgame::MainGame> CreateSinglePlayer()
{
	qfcbase::LevelLoader::SetFactory(std::make_shared<qfcbase::GameEntityFactory>());
	auto game = std::make_shared<qfcgame::MainGame>();
	auto scene = qfcbase::LevelLoader::LoadMap(game, 1, "Content/maps/Overworld01.tmx");
	game->LoadScene(scene, false);
	return game;
}

std::shared_ptr<qfcgame::MainGame> CreateMultiplayer(std::string auth_token)
{
	auto game = std::make_shared<qfcgame::MultiplayerGame>();
	game->Connect(0, auth_token);
	game->RefreshSceneFromServer();
	return game;
}
