#include <chrono>
#include <thread>
#include <iostream>

#include "Window.h"
#include "MainGame.h"
#include "ClientChannel.h"
#include "LevelLoader.h"
#include "GameEntityFactory.h"
#include "Controllable.h"
#include "KeyboardInput.h"

void SetupSinglePlayer(qfcgame::MainGame* game);
void SetupMultiplayer(qfcgame::MainGame* game, std::string auth_token);

int main(int argc, char* argv[])
{
	// Disables SFML errors messages, since we don't want all the "Could not load image" errors.
	std::cerr.rdbuf(0);
	sf::err().rdbuf(0);

	qfcbase::Window window({ 800, 600 }, "Quest for the Crown J");
	qfcgame::MainGame* game = new qfcgame::MainGame();

	// Loads Level INFO.
	qfcbase::LevelLoader::LoadLevels("Content/maps/QuestForTheCrown.maps");

	if (argc <= 1)
	{
		SetupSinglePlayer(game);
	}
	else
	{
		std::string auth_token = argv[1];
		SetupMultiplayer(game, auth_token);
	}

	window.Run(game);
	return 0;
}

void SetupSinglePlayer(qfcgame::MainGame* game)
{
	qfcbase::LevelLoader::SetFactory(new qfcbase::GameEntityFactory());
	auto scene = qfcbase::LevelLoader::LoadMap(game, 1, (std::string)"Content/maps/Overworld01.tmx");
	game->LoadScene(scene, false);

	bool found = false;
	scene->GetEntities([&found](const std::shared_ptr<qfcbase::Entity> e) {
		if (!found && e->category == "GoodGuy")
		{
			e->AddBehavior(std::make_shared<qfcgame::Controllable>(e, std::make_shared<qfcgame::KeyboardInput>()));
			found = true;
		}
		return false;
	});
}

void SetupMultiplayer(qfcgame::MainGame* game, std::string auth_token)
{
	game->Connect(12301, auth_token);
	game->RefreshSceneFromServer();
}
