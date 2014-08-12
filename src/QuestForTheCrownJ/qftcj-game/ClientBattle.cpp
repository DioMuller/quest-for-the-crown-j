#include "ClientBattle.h"
#include "GameAssets.h"
#include "AudioPlayer.h"

#include <sstream>
#include <SFML/Window/Keyboard.hpp>

using namespace qfcgame;
using namespace qfcbase;

ClientBattle::ClientBattle(std::weak_ptr<qfcbase::Game> parent) : qfcbase::BattleScene(parent)
{
	time = 0.0;
	lastAttack = 0.0;

	text = sf::Text("Battle!", *GameAssets::DefaultFont());
	text.setCharacterSize(12);
	text.setPosition(10, 10);
	

	AudioPlayer::PlayBGM("Firebrand");
}


ClientBattle::~ClientBattle()
{
}

void ClientBattle::Update(double dt)
{
	time += dt;

	// Battle Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) playerAction = BattleAction::ATTACK;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) playerAction = BattleAction::SPECIAL;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) playerAction = BattleAction::ITEM;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) playerAction = BattleAction::RUN;
	else playerAction = BattleAction::NOACTION;

	BattleScene::Update(dt);
}

void ClientBattle::Draw(sf::RenderWindow* renderer)
{
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
		<< "4 Run" << std::endl
		<< std::endl << std::endl;

	for (auto message : lastMessages)
	{
		sstream << message << std::endl;
	}
	str = sstream.str();
	text.setString(str);
	renderer->draw(text);

	//	Players Text
	int i = 0;
	for (auto entity : GetEntities())
	{
		auto battleEntity = std::static_pointer_cast<BattleEntity>(entity);

		if (battleEntity)
		{
			if (battleEntity->Type() == BattleEntityType::PLAYER)
				battleEntity->DrawInfo(renderer, sf::Vector2f(10.0f + (i * 200.0f), 550.0f));
			else if (battleEntity->Type() == BattleEntityType::ENEMY)
				battleEntity->DrawEntity(renderer, sf::Vector2f(350.0f + (i * 30.0f), 300.0f));
		}
		i++;
	}
}

bool ClientBattle::SelectAction(std::shared_ptr<qfcbase::BattleEntity> currentEntity)
{
	std::shared_ptr<BattleEntity> targetEntity;

	if (time - lastAttack < 0.1)
	{
		return false;
	}

	lastAttack = time;

	for (auto ent : GetEntities())
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

void ClientBattle::PrintMessage(std::string message)
{
	BattleScene::PrintMessage(message);
	lastMessages.push_back(message);
	while (lastMessages.size() > MAX_MESSAGES) lastMessages.pop_front();
}