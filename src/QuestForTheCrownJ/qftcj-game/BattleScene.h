#pragma once
#include "Scene.h"
#include "BattleEntity.h"
#include <SFML/Graphics/Text.hpp>

#define MAX_BATTLE_PLAYERS 4
#define MAX_BATTLE_ENEMIES 1

namespace qfcgame
{
	class BattleScene : public qfcbase::Scene
	{
		private:
			double time;
			sf::Text text;

			int currentTurn;
			std::vector<std::shared_ptr<BattleEntity>> players;
			std::vector<std::shared_ptr<BattleEntity>> enemies;

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

			bool PlayerJoin(std::shared_ptr<qfcbase::Entity> hero);
			bool AddMonster(std::shared_ptr<qfcbase::Entity> monster);
	};
}