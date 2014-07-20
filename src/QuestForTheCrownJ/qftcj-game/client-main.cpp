#include "Window.h"
#include "MainGame.h"

#include <chrono>
#include <thread>
#include <iostream>


int main(int argc, char* argv[])
{
	// Disables SFML errors messages, since we don't want all the "Could not load image" errors.
	std::cerr.rdbuf(0);
	sf::err().rdbuf(0);

	// id = arg[1]

	// request playerMap

	qfcbase::Window window({ 800, 600 }, "Quest for the Crown J");
	qfcgame::MainGame* game = new qfcgame::MainGame();

	game->LoadScene(game->CreateScene(game), false);

	window.Run(game);

	return 0;
}