#include "Window.h"
#include "MainGame.h"
#include <chrono>
#include <thread>

int main()
{
	qfcbase::Window window({ 800, 600 }, "Quest for the Crown J");
	qfcgame::MainGame* game = new qfcgame::MainGame();

	window.Run(game);

	return 0;
}