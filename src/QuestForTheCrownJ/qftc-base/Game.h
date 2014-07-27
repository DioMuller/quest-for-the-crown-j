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
		protected:
			Scene* currentScene;
		private:
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
			bool IsCurrentScene(Scene* scene);
	};
}