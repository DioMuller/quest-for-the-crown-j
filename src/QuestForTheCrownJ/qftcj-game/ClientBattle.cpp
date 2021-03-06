#include "ClientBattle.h"
#include "GameAssets.h"
#include "AudioPlayer.h"
#include "MultiplayerGame.h"
#include "Hero.h"

#include <sstream>
#include <SFML/Window/Keyboard.hpp>

using namespace qfcgame;
using namespace qfcbase;

ClientBattle::ClientBattle(std::weak_ptr<qfcbase::Game> parent, std::string background) : qfcbase::BattleScene(parent)
{
	time = 0.0;
	lastAttack = 0.0;
	requestInterval = 0.0;

	text = sf::Text("Battle!", *GameAssets::DefaultFont());
	battleCommands = sf::Text("Commands", *GameAssets::DefaultFont());
	
	text.setCharacterSize(12);
	text.setPosition(400, 500);

	battleCommands.setCharacterSize(20);
	battleCommands.setPosition(60, 420);
	battleCommands.setColor(sf::Color(255, 255, 0));

	if (!backgroundTexture.loadFromFile(background))
	{
		Log::Error("Error loading battle background image.");
	}
	else
	{
		Log::Message("Loaded background image: " + background);
		backgroundImage.setTexture(backgroundTexture);

		//TODO: Non-fixed size?
		backgroundImage.setScale(800.0f / backgroundTexture.getSize().x, 600.0f / backgroundTexture.getSize().y);
	}
	
	if (!messageBoxTexture.loadFromFile("Content/images/MessageBox.png"))
	{
		Log::Error("Error loading message box image.");
	}
	else
	{
		Log::Message("Loaded message box image.");
		messageBoxImage.setTexture(messageBoxTexture);

		//TODO: Non-fixed size?
		float scale = 800.0f / messageBoxTexture.getSize().x;
		messageBoxImage.setScale(scale, scale);
		messageBoxImage.setPosition(0, 600.0f - (messageBoxTexture.getSize().y * scale));
	}

	AudioPlayer::PlayBGM("Firebrand");
}


ClientBattle::~ClientBattle()
{
}

void ClientBattle::Update(double dt)
{
	time += dt;
	requestInterval += dt;

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

	renderer->draw(backgroundImage);
	renderer->draw(messageBoxImage);

	// Text Initialization.
	std::ostringstream bstream;
	std::ostringstream sstream;
	std::string str;

	// Battle Info Text
	bstream 
		//<< "Turn: " << currentTurn << std::endl
		<< "[1] Attack " << std::endl
		<< "[2] Magic" << std::endl
		<< "[3] Use Potion" << std::endl
		//<< "[4] Run" << std::endl
		<< std::endl << std::endl;

	str = bstream.str();
	battleCommands.setString(str);
	renderer->draw(battleCommands);

	for (auto message : lastMessages)
	{
		sstream << message << std::endl;
	}
	str = sstream.str();
	text.setString(str);
	renderer->draw(text);

	//	Players Text
	int i = 0;
	int j = 0;
	for (auto entity : GetEntities())
	{
		auto battleEntity = std::static_pointer_cast<BattleEntity>(entity);

		if (battleEntity)
		{
			if (battleEntity->Type() == BattleEntityType::PLAYER)
			{
				battleEntity->DrawInfo(renderer, sf::Vector2f(410.0f + (i * 200.0f), 420.0f));
				i++;
			}
			else if (battleEntity->Type() == BattleEntityType::ENEMY)
			{
				battleEntity->DrawInfo(renderer, sf::Vector2f(600.0f - (j * 200.0f), 420.0f));
				battleEntity->DrawEntity(renderer, sf::Vector2f(350.0f + (j * 30.0f), 250.0f));
				j++;
			}
		}
		
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
		if (playerAction == BattleAction::ITEM)
		{
			if (currentEntity->GetParent()->items.potions <= 0) 
				return false;
			else
			{
				targetEntity = currentEntity;
				currentEntity->GetParent()->items.potions--;
			}
		}

		nextTurn = Turn{ currentTurn, currentEntity, targetEntity, playerAction, 3 + rand() % 5 };
		SendTurn(playerAction, currentEntity->GetParent()->Id, targetEntity->GetParent()->Id, nextTurn.value);
		turns.push_back(nextTurn);
		return true;
		break;
	case BattleEntityType::ENEMY:
		if (receivedTurns.size() > 0)
		{
			for (auto turn : receivedTurns)
			{
				if (turn.id == currentTurn)
				{
					if (turn.entity->Type() != BattleEntityType::ENEMY)
						Log::Error("Client / Server disagree on battle turn");

					turns.push_back(turn);
					return true;
				}
			}
		}

		if (requestInterval > REQUEST_INTERVAL)
		{
			requestInterval = 0.0;
			RequestTurn();
			return false;
		}
		//nextTurn = Turn{ currentTurn, currentEntity, targetEntity, BattleAction::ATTACK, 3 + rand() % 5 };
		//break;
	default:
		return false;
	}
}

void ClientBattle::PrintMessage(std::string message)
{
	BattleScene::PrintMessage(message);
	lastMessages.push_back(message);
	while (lastMessages.size() > MAX_MESSAGES) lastMessages.pop_front();
}

void ClientBattle::RequestTurn()
{
	auto sceneParent = std::dynamic_pointer_cast<MultiplayerGame>(parent.lock());

	if (sceneParent)
	{
		sceneParent->RequestTurn(currentTurn);
	}
}

void ClientBattle::SendTurn(BattleAction command, int entity, int target, int additional_info)
{
	auto sceneParent = std::dynamic_pointer_cast<MultiplayerGame>(parent.lock());

	if (sceneParent)
	{
		sceneParent->SendTurn(currentTurn, command, entity, target, additional_info);
	}
}

void ClientBattle::ReceiveTurn(qfcbase::BattleAction command, int turn_id, int additional_info, int entity_id, int target_id)
{
	std::shared_ptr<BattleEntity> currentEntity = nullptr;
	std::shared_ptr<BattleEntity> targetEntity = nullptr;

	for (auto entity : GetEntities())
	{
		auto bent = std::static_pointer_cast<BattleEntity>(entity);
		if ( bent )
		{
			if (bent->GetParent()->Id == entity_id) currentEntity = bent;
			if (bent->GetParent()->Id == target_id) targetEntity = bent;
		}
	}

	if ( currentEntity && targetEntity )
		receivedTurns.push_back(Turn{ turn_id, currentEntity, targetEntity, command, additional_info });
	else {
		if (!currentEntity)
			Log::Error("Unknown battle entity: " + std::to_string(entity_id));
		else
			Log::Error("Unknown battle target: " + std::to_string(target_id));
	}
}