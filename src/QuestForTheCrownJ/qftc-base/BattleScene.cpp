#include "BattleScene.h"
#include <string>
#include "Game.h"

#include "Log.h"

using namespace qfcbase;

BattleScene::BattleScene(std::weak_ptr<qfcbase::Game> parent)
	: Scene(parent, false)
{
	currentTurn = 0;

	enemyCount = 0;
	playerCount = 0;
}


BattleScene::~BattleScene()
{
}

void BattleScene::Update(double dt)
{
	std::shared_ptr<Scene> battle = getptr();
	
	if (enemyCount <= 0 || playerCount <= 0)
	{
		auto game = std::dynamic_pointer_cast<Game>(parent.lock());
		if (game)
		{
			for (auto e : GetEntities())
				game->UnstackScene(std::dynamic_pointer_cast<BattleEntity>(e)->GetParent());
		}
	}

	// TODO: Correct Turn Treatment.
	NextTurn();
}

void BattleScene::Draw(sf::RenderWindow* renderer)
{

}

bool BattleScene::PlayerJoin(std::shared_ptr<Entity> hero)
{
	if (playerCount > MAX_BATTLE_PLAYERS) return false;
	
	playerCount++;
	AddEntity(std::shared_ptr<BattleEntity>(new BattleEntity(hero, BattleEntityType::PLAYER)));
	return true;
}

bool BattleScene::AddMonster(std::shared_ptr<Entity> monster)
{
	if (enemyCount > MAX_BATTLE_ENEMIES) return false;
	
	enemyCount++;
	AddEntity(std::shared_ptr<BattleEntity>(new BattleEntity(monster, BattleEntityType::ENEMY)));
	return true;
}

void BattleScene::NextTurn()
{
	auto entities = GetEntities();

	if (entities.size() == 0) return;
	if (playerCount <= 0 || enemyCount <= 0) return;

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
	if (SelectAction(turnOrder[turnOrder.size() - 1]))
		ExecuteTurn();
}

bool BattleScene::SelectAction(std::shared_ptr<BattleEntity> currentEntity)
{
	return false;
}

void BattleScene::ExecuteTurn()
{
	std::shared_ptr<BattleEntity> entity, target;
	int value;

	turnOrder.pop_back();

	entity = turns[currentTurn].entity;
	target = turns[currentTurn].target;
	value = turns[currentTurn].value;

	if (!target) return;

	switch (turns[currentTurn].action)
	{
		case BattleAction::ATTACK:
			PrintMessage(entity->category + " attacked " + target->category);
			PrintMessage("Damage " + std::to_string(value));

			target->ChangeHP(-value);
			break;
		case BattleAction::SPECIAL:
			PrintMessage(entity->category + " special attacked " + target->category);
			PrintMessage("Damage " + std::to_string(value));

			target->ChangeHP(-value);
			break;
		case BattleAction::ITEM:
			PrintMessage(entity->category + " used potion on " + target->category);
			PrintMessage("Cured " + std::to_string(value));

			target->ChangeHP(value);
			break;
		case BattleAction::RUN:
			PrintMessage(entity->category + " ran away!");
			RemoveEntity(entity);
			if (entity->Type() == BattleEntityType::ENEMY) enemyCount--;
			else if (entity->Type() == BattleEntityType::PLAYER) playerCount--;
			break;
		case BattleAction::NOACTION:
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

	currentTurn++;
}

void BattleScene::PrintMessage(std::string message)
{
	Log::Message(message);
}
