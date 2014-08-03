#include "BattleScene.h"

#include <string>
#include <sstream>
#include <SFML/Window/Keyboard.hpp>

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

	// Battle Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) playerAction = BattleAction::ATTACK;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) playerAction = BattleAction::SPECIAL;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) playerAction = BattleAction::ITEM;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) playerAction = BattleAction::RUN;
	else playerAction = BattleAction::NOACTION;

	// TODO: Correct Turn Treatment.
	NextTurn();
}

void BattleScene::Draw(sf::RenderWindow* renderer)
{
	int i;

	// Camera
	sf::Vector2f screenSize = sf::Vector2f(renderer->getSize().x, renderer->getSize().y);
	sf::Vector2f cameraPosition = sf::Vector2f(screenSize.x / 2, screenSize.y / 2);
	renderer->setView(sf::View(cameraPosition, screenSize));

	// Text Initialization.
	std::ostringstream sstream;
	std::string str;

	// Battle Info Text
	sstream << "1 Attack " << std::endl
		<< "2 Magic" << std::endl
		<< "3 Use Potion" << std::endl
		<< "4 Run" << std::endl;

	for (i = 0; i < MAX_MESSAGES; i++)
	{
		sstream << lastMessages[i] << std::endl;
	}
	str = sstream.str();
	text.setString(str);
	renderer->draw(text);

	//	Players Text
	for (i = 0; i < entities.size(); i++)
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
	if (entities.size() == 0) return;

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
	std::shared_ptr<BattleEntity> targetEntity;

	if (time - lastAttack < 0.1)
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

	Turn nextTurn;

	switch (currentEntity->Type())
	{
		case BattleEntityType::PLAYER:
			if (playerAction == BattleAction::NOACTION) return false;
			if ((playerAction == BattleAction::SPECIAL && currentEntity->CurrentMP() <= 0)) return false;
			// TODO: Potions.
			
			nextTurn = Turn{ currentTurn, currentEntity, targetEntity, playerAction, 3 + rand() % 5 };
			break;
		case BattleEntityType::ENEMY:
			nextTurn = Turn{ currentTurn, currentEntity, targetEntity, BattleAction::ATTACK, 3 + rand() % 5 };
			break;
		default:
			return false;
	}

	turns.push_back(nextTurn);

	return true;

}

void BattleScene::ExecuteTurn()
{
	std::shared_ptr<BattleEntity> entity, target;
	int value;

	turnOrder.pop_back();

	entity = turns[currentTurn].entity;
	target = turns[currentTurn].target;
	value = turns[currentTurn].value;

	switch (turns[currentTurn].action)
	{
		case BattleAction::ATTACK:
			PrintMessage(entity->category + " attacked " + target->category);
			PrintMessage("Damage: " + std::to_string(value));

			target->ChangeHP(-value);
			break;
		case BattleAction::SPECIAL:
			PrintMessage(entity->category + " special attacked " + target->category);
			PrintMessage("Damage: " + std::to_string(value));

			target->ChangeHP(-value);
			break;
		case BattleAction::ITEM:
			PrintMessage(entity->category + " used potion on " + target->category);
			PrintMessage("Cured: " + std::to_string(value));

			target->ChangeHP(value);
			break;
		case BattleAction::RUN:
			PrintMessage(entity->category + " ran away!");
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
	lastMessages[currentMessage] = message;
	currentMessage++;
	currentMessage %= MAX_MESSAGES;
}
