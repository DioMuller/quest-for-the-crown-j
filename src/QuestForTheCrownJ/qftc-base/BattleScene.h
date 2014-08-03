#pragma once
#include "Scene.h"
#include "BattleEntity.h"
#include <SFML/Graphics/Text.hpp>

#define MAX_BATTLE_PLAYERS 1
#define MAX_BATTLE_ENEMIES 1

#define MAX_MESSAGES 4

namespace qfcbase
{
	enum BattleAction
	{
		ATTACK = 0,
		SPECIAL = 1,
		ITEM = 2,
		RUN = 3,
		NOACTION = -1
	};

	typedef struct structTurn
	{
		int id; // Turn Count
		std::shared_ptr<BattleEntity> entity; // Entity
		std::shared_ptr <BattleEntity> target;
		BattleAction action; // Turn Action
		int value; // Value (Damage, Restore, Success)
	} Turn;

	class BattleScene : public Scene
	{
		private:
			int currentTurn;
			std::vector<Turn> turns;

			int playerCount;
			int enemyCount;

			std::vector<std::shared_ptr<BattleEntity>> turnOrder;
			
			// To Single Player
			double time;
			double lastAttack;
			BattleAction playerAction;
			std::string lastMessages[MAX_MESSAGES]; // TODO: Change to Queue
			int currentMessage = 0;
			sf::Text text;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			BattleScene(std::weak_ptr<qfcbase::Game> parent);
			~BattleScene();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			void Update(double dt);
			virtual void Draw(sf::RenderWindow* renderer);

			bool PlayerJoin(std::shared_ptr<Entity> hero);
			bool AddMonster(std::shared_ptr<Entity> monster);

			void NextTurn();
			virtual bool SelectAction(std::shared_ptr<BattleEntity> entity);
			void ExecuteTurn();

			void PrintMessage(std::string message);
	};
}