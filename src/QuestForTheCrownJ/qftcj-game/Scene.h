#pragma once
#include <stack>
#include "Entity.h"

namespace qfcbase
{
	class Scene
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			std::vector<Entity*> entities;
			std::stack<Entity*> toRemove;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Scene();
			~Scene();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			virtual void Update(double dt);
			virtual void Draw(sf::RenderWindow* renderer);

			void AddEntity(Entity* e);
			void RemoveEntity(Entity* e);
	};
}