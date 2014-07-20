#include "Window.h"
#include "MainGame.h"

#include <chrono>
#include <thread>
#include <iostream>
#include "ClientChannel.h"

int main(int argc, char* argv[])
{
	// Disables SFML errors messages, since we don't want all the "Could not load image" errors.
	std::cerr.rdbuf(0);
	sf::err().rdbuf(0);

	if (argc <= 1)
	{
		std::cout << "Use the Launcher to play the game\n";
		return 1;
	}

	std::string auth_token = argv[1];

	qfcbase::Window window({ 800, 600 }, "Quest for the Crown J");
	qfcgame::MainGame* game = new qfcgame::MainGame(auth_token);

	game->RefreshSceneFromServer();
	window.Run(game);

	return 0;
}