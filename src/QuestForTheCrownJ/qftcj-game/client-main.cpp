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

	std::string auth_token; // = arg[1]
	PlayerInfo player; // = clientChannel.GetPlayerInfo(auth_token)
	std::vector<EntityInfo> entities; // = clientChannel.GetEntities(auth_token, player.map_name)

	qfcbase::Window window({ 800, 600 }, "Quest for the Crown J");
	qfcgame::MainGame* game = new qfcgame::MainGame();

	game->LoadScene(player, entities);

	window.Run(game);

	return 0;
}