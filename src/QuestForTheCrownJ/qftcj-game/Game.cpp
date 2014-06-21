#include "Game.h"

using namespace qfcbase;

Game::Game()
{
	currentScene = nullptr;
}


Game::~Game()
{
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
void Game::Update(double dt)
{
	if( currentScene != nullptr ) currentScene->Update(dt);
}

void Game::Draw(sf::RenderWindow* renderer)
{
	if (currentScene != nullptr) currentScene->Draw(renderer);
}

void Game::LoadScene(Scene* s, bool stack)
{
	if (s == currentScene) return;

	if (stack)
	{
		pastScenes.push(currentScene);
	}
	else
	{
		delete currentScene;
	}

	currentScene = s;
}

void Game::UnstackScene()
{
	delete currentScene;
	currentScene = (Scene*)pastScenes.top();
	pastScenes.pop();
}