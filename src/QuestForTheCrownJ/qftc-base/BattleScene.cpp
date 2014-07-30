#include "BattleScene.h"

#include <string>
#include <sstream>

#include "Game.h"
#include "GameAssets.h"
#include "AudioPlayer.h"

using namespace qfcbase;

BattleScene::BattleScene(std::weak_ptr<qfcbase::Game> parent)
	: Scene(parent, false)
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

	if (enemies.size() <= 0)
	{
		auto game = std::dynamic_pointer_cast<Game>(parent.lock());
		if (game)
		{
			game->UnstackScene(players[0]->Parent());
		}
	}
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
		players[i]->DrawInfo(renderer, sf::Vector2f(10.0f + (i * 200.0f), 550.0f));
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

void BattleScene::NextTurn()
{
	if (turnOrder.size() == 0)
	{
		for (auto player : players)
		{
			turnOrder.push_back(player);
		}

		for (auto enemy : enemies)
		{
			turnOrder.push_back(enemy);
		}

		std::sort(turnOrder.begin(), turnOrder.end(), 
			[]( std::shared_ptr<BattleEntity> p1, 
			    std::shared_ptr<BattleEntity> p2 )
			{
				return p1->Speed < p2->Speed;
			}
		);
	}

	currentTurn++;

	// TODO: Turn Logic!
	turns.push_back(Turn{ currentTurn, turnOrder[turnOrder.size() - 1], BattleAction::ATTACK, rand() % 10 });

	turnOrder.pop_back();
}