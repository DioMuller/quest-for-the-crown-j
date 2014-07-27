#include "BattleScene.h"
#include "Game.h"
#include "GameAssets.h"
#include "AudioPlayer.h"
#include <string>
#include <sstream>

using namespace qfcbase;

BattleScene::BattleScene(Game* parent) : Scene(parent, false)
{
	time = 0.0;

	text = sf::Text("Battle!", *GameAssets::DefaultFont());
	text.setCharacterSize(12);
	text.setPosition(10, 10);
	currentTurn = 0;

	AudioPlayer::PlayBGM("Firebrand");
}


BattleScene::~BattleScene()
{
}

void BattleScene::Update(double dt)
{
	time += dt;

	if ( time > 5.0)
		parent->UnstackScene();
}

void BattleScene::Draw(sf::RenderWindow* renderer)
{
	// Camera
	sf::Vector2f screenSize = sf::Vector2f(renderer->getSize().x, renderer->getSize().y);
	sf::Vector2f cameraPosition = sf::Vector2f(screenSize.x / 2, screenSize.y / 2);
	renderer->setView(sf::View(cameraPosition, screenSize));

	// Text Initialization.
	std::ostringstream sstream;
	std::string str;

	// Battle Info Text
	sstream << "Battle Time " << (int)time << std::endl
		<< "Players " << players.size() << std::endl
		<< "Enemies " << enemies.size();
	str = sstream.str();
	text.setString(str);
	renderer->draw(text);

	//	Players Text
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->DrawInfo(renderer, sf::Vector2f(10.0f + (i * 200.0f), 500.0f));
	}

	//	Enemy Sprite
	for (int i = 0; i < players.size(); i++)
	{
		enemies[i]->DrawEntity(renderer, sf::Vector2f(350.0f + (i * 30.0f), 300.0f));
	}
}

bool BattleScene::PlayerJoin(std::shared_ptr<Entity> hero)
{
	if (players.size() > MAX_BATTLE_PLAYERS) return false;
	
	players.push_back(std::shared_ptr<BattleEntity>(new BattleEntity(hero)));
	return true;
}

bool BattleScene::AddMonster(std::shared_ptr<Entity> monster)
{
	if (enemies.size() > MAX_BATTLE_ENEMIES) return false;

	enemies.push_back(std::shared_ptr<BattleEntity>(new BattleEntity(monster)));
	return true;
}