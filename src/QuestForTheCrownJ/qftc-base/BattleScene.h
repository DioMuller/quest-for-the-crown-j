#pragma once
#include "Scene.h"
#include "BattleEntity.h"
#include <SFML/Graphics/Text.hpp>

#define MAX_BATTLE_PLAYERS 4
#define MAX_BATTLE_ENEMIES 1

namespace qfcbase
{
	enum BattleAction
	{
		ATTACK = 0,
		SPECIAL = 1,
		ITEM = 2,
		RUN = 3
	};

	typedef struct structTurn
	{
		int id; // Turn Count
		std::shared_ptr<BattleEntity> entity; // Entity
		BattleAction action; // Turn Action
		int value; // Value (Damage, Restore, Success)
	} Turn;

	class BattleScene : public Scene
	{
		private:
			double time;
			sf::Text text;

			std::vector<std::shared_ptr<BattleEntity>> players;
			std::vector<std::shared_ptr<BattleEntity>> enemies;

			int currentTurn;
			std::vector<Turn> turns;

			std::vector<std::shared_ptr<BattleEntity>> turnOrder;

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
			void Draw(sf::RenderWindow* renderer);

			bool PlayerJoin(std::shared_ptr<Entity> hero);
			bool AddMonster(std::shared_ptr<Entity> monster);

			void NextTurn();
	};
}