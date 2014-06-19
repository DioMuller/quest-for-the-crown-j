#include "Game.h"

using namespace qfcbase;

Game::Game()
{
}


Game::~Game()
{
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
void Game::Update(double dt)
{

}

void Game::Draw(sf::RenderWindow* renderer)
{

}

void Game::LoadScene(Scene* s, bool stack)
{
	if (s == currentScene) return;

	if (stack)
	{
		pastScenes.push(currentScene);
	}
}

void Game::StackCurrentScene()
{

}