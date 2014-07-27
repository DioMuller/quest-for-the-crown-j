#pragma once
#include "Entity.h"

namespace qfcbase
{
	class BattleEntity : public Entity
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			std::shared_ptr<Entity> parent;
			sf::Text text;

		/////////////////////////////////////
		// "Properties"
		/////////////////////////////////////
		public:
			std::shared_ptr<Entity> Parent()
			{
				return parent;
			}
		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			BattleEntity(std::shared_ptr<Entity> parent);
			~BattleEntity();

		public:
			void DrawEntity(sf::RenderWindow* renderer, sf::Vector2f position);
			void DrawInfo(sf::RenderWindow* renderer, sf::Vector2f position);
	};
}