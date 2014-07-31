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

		/////////////////////////////////////
		// "Properties"
		/////////////////////////////////////
		public:
			std::shared_ptr<qfcbase::Entity> Parent()
			{
				return parent;
			}

			BattleEntityType Type()
			{
				return type;
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