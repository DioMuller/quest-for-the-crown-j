#include "BattleScene.h"

#include <string>
#include <sstream>

#include "Game.h"
#include "GameAssets.h"
#include "AudioPlayer.h"
#include "Log.h"

using namespace qfcbase;

BattleScene::BattleScene(std::weak_ptr<qfcbase::Game> parent)
	: Scene(parent, false)
{
	time = 0.0;
	lastAttack = 0.0;

	enemyCount = 0;
	playerCount = 0;

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

	if (enemyCount <= 0 || playerCount <= 0)
	{
		auto game = std::dynamic_pointer_cast<Game>(parent.lock());
		if (game)
		{
			game->UnstackScene(std::dynamic_pointer_cast<BattleEntity>(entities[0])->GetParent());
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
		<< "Entities " << entities.size();
	str = sstream.str();
	text.setString(str);
	renderer->draw(text);

	//	Players Text
	for (int i = 0; i < entities.size(); i++)
	{
		auto battleEntity = std::static_pointer_cast<BattleEntity>(entities[i]);
		
		if ( battleEntity )
		{
			if (battleEntity->Type() == BattleEntityType::PLAYER)
				battleEntity->DrawInfo(renderer, sf::Vector2f(10.0f + (i * 200.0f), 550.0f));
			else if (battleEntity->Type() == BattleEntityType::ENEMY)
				battleEntity->DrawEntity(renderer, sf::Vector2f(350.0f + (i * 30.0f), 300.0f));
		}
	}
	// TODO: Correct Turn Treatment.
	NextTurn();
}

bool BattleScene::PlayerJoin(std::shared_ptr<Entity> hero)
{
	if (playerCount > MAX_BATTLE_PLAYERS) return false;
	
	playerCount++;
	entities.push_back(std::shared_ptr<BattleEntity>(new BattleEntity(hero, BattleEntityType::PLAYER)));
	return true;
}

bool BattleScene::AddMonster(std::shared_ptr<Entity> monster)
{
	if (enemyCount > MAX_BATTLE_ENEMIES) return false;
	
	enemyCount++;
	entities.push_back(std::shared_ptr<BattleEntity>(new BattleEntity(monster, BattleEntityType::ENEMY)));
	return true;
}

void BattleScene::NextTurn()
{
	if (turnOrder.size() == 0)
	{
		for (auto entity : entities)
		{
			auto battle = std::static_pointer_cast<BattleEntity>(entity);
			turnOrder.push_back(battle);
		}

		std::sort(turnOrder.begin(), turnOrder.end(), 
			[]( std::shared_ptr<BattleEntity> p1, 
			    std::shared_ptr<BattleEntity> p2 )
			{
				return p1->Speed < p2->Speed;
			}
		);
	}

	// TODO: Turn Logic!
	if (ExecuteTurn(turnOrder[turnOrder.size() - 1])) currentTurn++;
}

bool BattleScene::ExecuteTurn(std::shared_ptr<BattleEntity> currentEntity)
{
	std::shared_ptr<BattleEntity> targetEntity;
	std::shared_ptr<BattleEntity> entity, target;
	int value;

	if (time - lastAttack < 1.0)
	{
		return false;
	}

	lastAttack = time;

	for (auto ent : entities)
	{
		auto be = std::static_pointer_cast<BattleEntity>(ent);
		if (be && be->Type() != currentEntity->Type())
		{
			targetEntity = be;
			break;
		}
	}

	if (!targetEntity) return false;

	switch (currentEntity->Type())
	{
		case BattleEntityType::ENEMY:
		case BattleEntityType::PLAYER:
			turns.push_back(Turn{ currentTurn, currentEntity, targetEntity, BattleAction::ATTACK, rand() % 10 });
			turnOrder.pop_back();

			entity = turns[currentTurn].entity;
			target = turns[currentTurn].target;
			value = turns[currentTurn].value;

			switch (turns[currentTurn].action)
			{
				case BattleAction::ATTACK:
					Log::Message(entity->category + " attacked " + target->category);
					Log::Message("Damage: " + std::to_string(value));

					target->ChangeHP(-value);
					break;
				case BattleAction::SPECIAL:
					Log::Message(entity->category + " special attacked " + target->category);
					Log::Message("Damage: " + std::to_string(value));

					target->ChangeHP(-value);
					break;
				case BattleAction::ITEM:
					Log::Message(entity->category + " used potion on " + target->category);
					Log::Message("Cured: " + std::to_string(value));

					target->ChangeHP(value);
					break;
				case BattleAction::RUN:
					Log::Message(entity->category + " ran away!");


					break;
				default:
					Log::Warning("Unknown Action!");
			}

			if (target->CurrentHP() <= 0)
			{
				RemoveEntity(target);

				if (target->Type() == BattleEntityType::ENEMY) enemyCount--;
				else if (target->Type() == BattleEntityType::PLAYER) playerCount--;
			}

			return true;
		default:
			return false;
	}

}