#pragma once
#include "Entity.h"

namespace qfcbase
{
	enum BattleEntityType
	{
		PLAYER = 0,
		ENEMY = 1
	};

	class BattleEntity : public qfcbase::Entity
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			std::shared_ptr<qfcbase::Entity> parent;
			BattleEntityType type;
			sf::Text text;
			int current_hp;
			int current_mp;

		/////////////////////////////////////
		// "Properties"
		/////////////////////////////////////
		public:
			BattleEntityType Type()
			{
				return type;
			}

			std::shared_ptr<qfcbase::Entity> GetParent()
			{
				return parent;
			}

			int CurrentHP()
			{
				return current_hp;
			}

			int CurrentMP()
			{
				return current_mp;
			}

			void ChangeHP(int quantity)
			{
				current_hp = max(current_hp + quantity, 0);
				current_hp = min(current_hp, parent->status.HP);
			}

			void ChangeMP(int quantity)
			{
				current_mp = max(current_mp - quantity, 0);
				current_mp = min(current_mp, parent->status.MP);
			}
		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			BattleEntity(std::shared_ptr<qfcbase::Entity> parent, BattleEntityType type);
			~BattleEntity();

		public:
			void DrawEntity(sf::RenderWindow* renderer, sf::Vector2f position);
			void DrawInfo(sf::RenderWindow* renderer, sf::Vector2f position);
	};
}