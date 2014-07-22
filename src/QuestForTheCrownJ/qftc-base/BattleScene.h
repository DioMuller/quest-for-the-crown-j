#pragma once
#include "Scene.h"
#include <SFML/Graphics/Text.hpp>

namespace qfcbase
{
	class BattleScene : public Scene
	{
		private:
			double time;
			sf::Font font;
			sf::Text text;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			BattleScene(Game* parent);
			~BattleScene();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			void Update(double dt);
			void Draw(sf::RenderWindow* renderer);
	};
}