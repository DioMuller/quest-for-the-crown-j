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

	qfcbase::Window window({ 800, 600 }, "Quest for the Crown J");
	qfcgame::MainGame* game = new qfcgame::MainGame();

	window.Run(game);

	return 0;
}