#pragma once

#include <stack>
#include "Scene.h"

namespace qfcbase
{
	class Game
	{
		////////////////////////////////////////
		// Attributes
		////////////////////////////////////////
		private:
			Scene* currentScene;
			std::stack<Scene*> pastScenes;
			bool endScene;
			bool sceneStacked;

		////////////////////////////////////////
		// Constructors
		////////////////////////////////////////
		public:
			Game();
			~Game();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			virtual void Update(double dt);
			virtual void Draw(sf::RenderWindow* renderer);

			void LoadScene(Scene* s, bool stack = false);
			void UnstackScene();
	};
}