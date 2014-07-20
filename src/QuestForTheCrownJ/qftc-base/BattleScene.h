#pragma once
#include "Scene.h"
namespace qfcbase
{
	class BattleScene : public Scene
	{
		private:
			double time;

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